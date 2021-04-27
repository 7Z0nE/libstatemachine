#ifndef EVENT_DATA_H
#define EVENT_DATA_H
class EventData {
public:
    virtual ~EventData() {}
};

class EmptyData : public EventData {
};


#endif //EVENT_DATA_H