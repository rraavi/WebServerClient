CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		udp_client.o

LIBS =

TARGET =	udp_client

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
