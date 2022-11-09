size_msg = connected_socket.recv(4)
        if not size_msg:
            break; # end of the connection from client

        (size, ) = struct.unpack('!I', size_msg)

        # get entry message
# get entry message
entry_message = connected_socket.recv(size)
if len(entry_message) != size:
    raise IOError('Server partial read for entry message : ' +
                    str(len(entry_message)) + ' instead of ' +
                    str(size))

entry = survey_pb2.Survey()
entry.ParseFromString(entry_message)


"""========================================================="""

# build Competitor
competitor = Competitor()
competitor.lastname = "Fix"
competitor.firstname = "John"
competitor.birth_date.year = 1980
competitor.birth_date.month = 7
competitor.birth_date.day = 12

"""
captain = from_mars.rescue_demand.astronaut.add()
captain.name = "Captain"
captain.health_status = Astronaut.AILING
crew_member = from_mars.rescue_demand.astronaut.add()
crew_member.name = "Gary"
crew_member.health_status = Astronaut.GOOD

if from_earth.WhichOneof("type") != "rescue_offer":
    raise RuntimeError("Rescue offer is the only message that I'm ready to recv")
"""