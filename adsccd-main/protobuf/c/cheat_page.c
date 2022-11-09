


Survey entry;

/* init entry */
survey__init(&entry);
entry.size_in_cm = rand_r(&seed) % MAX_SIZE_IN_CM;
if (rand_r(&seed) % 2) {
    entry.gender = GENDER__MALE;
    fprintf(stdout, "Client : %d entry, male, %d cm.\n", 
            i, entry.size_in_cm);
} else {
    entry.gender = GENDER__FEMALE;
    fprintf(stdout, "Client : %d entry, female, %d cm.\n", 
            i, entry.size_in_cm);
}

/* alloc buf */
msg_size = survey__get_packed_size(&entry);
buf = malloc(msg_size);
if (!buf) {
    fprintf(stderr, "Client unable to alloc buf.\n");
    goto clean;
}

/* fill buf */
nb_bytes = survey__pack(&entry, buf);
if (nb_bytes != msg_size) {
    fprintf(stderr, "Client error on packing entry : %ld instead of %d "
                        ".\n", nb_bytes, msg_size);
    goto clean;
}

/* send size */
/* send entry */

/* free entry */
    free(buf); buf = NULL;



/*=============================================================*/

Survey *entry;
/* read survey entry message */
nb_bytes = recv(connected_socket, buf, msg_size, 0);
if (nb_bytes == -1) {
    fprintf(stderr, "Server error when receiving message: %d , %s .\n",
                    errno, strerror(errno));
    goto clean;
}

/* check partial read */
if (nb_bytes != msg_size) {
    fprintf(stderr, "Server got a partial read on message: %ld "
                    "instead of %d .\n", nb_bytes, msg_size);
    goto clean;
}

/* store survey entry */
entry = survey__unpack(NULL, msg_size, buf);
if (!entry) {
    fprintf(stderr, "Server error when unpacking a message.\n");
    goto clean;
}

/* free buffer and message */
free(buf); buf = NULL;
survey__free_unpacked(entry, NULL);
