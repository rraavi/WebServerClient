CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		udp_server.o

LIBS =

TARGET =	udp_server

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
