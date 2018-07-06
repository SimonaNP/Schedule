#include <iostream>
#include <cmath>
#include <string.h>


class Time
{
public:
    Time():hours(0), minutes(0) {}

    Time(int _hours, int _minutes):hours(_hours), minutes(_minutes)
    {
        setCorrectTime();
    }


    void setHours(const int _hours)
    {
        hours = _hours;
        setCorrectTime();
    }

    void setMinutes(const int _minutes)
    {
        minutes = _minutes;
        setCorrectTime();
    }


    const unsigned getHours()const
    {
        return hours;
    }

    const unsigned getMinutes() const
    {
        return minutes;
    }


    void reset()
    {
        hours = 0;
        minutes = 0;
    }

    // int + Time
    friend Time operator+(int left, Time const & right)
    {
        int mins = right.getMins() + left;
        int _minutes = mins % 60;
        int _hours = mins/60;
        return Time(_hours, _minutes);
    }

    // Time + int
    friend Time operator+(Time const& left, int right)
    {
        return right + left;
    }

    // Time + Time
    friend Time operator+(Time const& left, Time const& right)
    {
        return Time((int)left.getHours()+ (int)right.getHours(),(int)left.getMinutes()+ (int)right.getMinutes());
    }

    const int getMins() const
    {
        return 60*hours + minutes;
    }


    int operator-(const Time& t)
    {
        return std::abs(this->getMins() - t.getMins());
    }

    //Time+=int
    Time& operator+=(const int _minutes)
    {
        return *this = *this + (int)_minutes;
    }

    //Time+=Time
    Time& operator+=(const Time& t)
    {
        return *this = *this + t;
    }

    // prefix ++
    Time& operator++()
    {
        return *this += 1;
    }

    // postfix ++
    Time operator++(int)
    {
        Time oldValue = *this;
        ++*this;

        return oldValue;
    }

    const bool operator==(const Time& t) const
    {
        return hours==t.getHours() && minutes==t.getMinutes();
    }

    const bool operator<(const Time& t) const
    {

        return getMins()<t.getMins();
    }

    const bool operator<=(const Time& t) const
    {

        return *this<t || *this==t;
    }

    const bool operator>(const Time& t) const
    {

        return ! (*this<=t);
    }

    const bool operator>=(const Time& t) const
    {

        return ! (*this<t);
    }


    friend std::ostream& operator<<(std::ostream& out, const Time& t)
    {
        if(t.getHours()<10)
            out<<'0';
        out<<t.getHours()<<':';
        if(t.getMinutes()<10)
            out<<'0';
        out<<t.getMinutes();
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Time& t)
    {
        std::cout<<"Enter hours: ";
        in>>t.hours;
        std::cout<<"Enter minutes: ";
        in>>t.minutes;
        t.setCorrectTime();
        return in;
    }

private:
    unsigned hours;
    unsigned minutes;

    void setCorrectTime()
    {
        if(minutes >=60)
        {
            hours += minutes/60;
            minutes %= 60;
        }

        if(hours >= 24)
            hours %= 24;
    }

};

class Appointment
{
public:
    Appointment():time(),person(nullptr) {}

    Appointment( Time& _time,  char* _person):time(_time),person(_person) {}

    Appointment(Appointment const& other)
    {
        copy(other);
    }

    Appointment& operator=(Appointment const& other)
    {
        if(this!= &other)
            copy(other);
        return *this;
    }

    ~Appointment()
    {
        delete[] person;
    }

    const Time getTime() const
    {
        return time;
    }

    const char* getPerson() const
    {
        return person;
    }

    void setTime(Time const& _time)
    {
        time = _time;
    }

    void setPerson(char const* _person)
    {
        if (_person == nullptr)
            _person = "";

        int len = strlen(_person) + 1;
        delete[] this->person;
        this->person = new char[len];
        strncpy(this->person, _person, len);
    }

    friend std::ostream& operator<<(std::ostream& out, Appointment const& appointment)
    {
        return out<<appointment.getTime()<<" - "<<appointment.getPerson();

    }

private:
    Time time;
    char* person;

    void copy(const Appointment& app)
    {
        setTime(app.getTime());
        setPerson(app.getPerson());
    }


};

class Schedule
{
public:
    Schedule():size(0),capacity()
    {
        container = new Appointment[1];
    }

    Schedule(int _capacity):size(0),capacity(_capacity)
    {
        container = new Appointment[capacity];
    }

    Schedule(const Schedule& other)
    {
        copy(other);
    }

    Schedule& operator=(const Schedule& other)
    {
        if(this!=&other)
            this->copy(other);
        return *this;
    }

    ~Schedule()
    {
        delete[] container;
    }


    const unsigned getSize() const
    {
        return size;
    }

    const unsigned getCapacity() const
    {
        return capacity;
    }


    const Appointment* getContainer() const
    {
        return container;
    }


    void addAppointment(const Appointment& appointment)
    {
        if(size==capacity)
            resize();
        container[size++] = appointment;

    }

    friend std::ostream& operator<<(std::ostream& out, const Schedule& schedule)
    {
        for(int i = 0; i< schedule.getSize(); i++)
        {
            out<<schedule.getContainer()[i]<<std::endl;
        }
        return out;
    }

    Appointment& operator[](const int x)
    {
        return container[x];
    }


    void clearBetween(Time const& t1, Time const& t2)
    {
        int minTime = std::min(t1.getMins(),t2.getMins());
        int maxTime = std::max(t1.getMins(),t2.getMins());
        for(int i=0; i<size; i++)
        {
            if(container[i].getTime().getMins()>=minTime &&container[i].getTime().getMins()<=maxTime )
            {
                container[i]=container[--size];
                --i;
            }
        }
    }



    void clear()
    {
        for(int i=0; i<size; i++)
        {
            Appointment temp = container[i];
            this->clearBetween(temp.getTime()+30, temp.getTime()+(-30));
            addAppointment(temp);
        }
    }


    //BubbleSort
    void sort()
    {
        for (int i = 0; i < size-1; i++)

            // Last i elements are already in place
            for (int j = 0; j < size-i-1; j++)
                if (container[j].getTime() > container[j+1].getTime())
                    this->swap(&container[j], &container[j+1]);
    }


private:
    unsigned size;
    unsigned capacity;
    Appointment* container;

    void setSize(const int _size)
    {
        size=_size;
    }

    void  setCapacity(const int _capacity)
    {
        capacity=_capacity;
    }

    void copy(Schedule const& schedule)
    {
        setSize(schedule.getSize());
        setCapacity(schedule.getCapacity());
        container = new Appointment[capacity];
        for(int i=0; i<size; i++)
        {
            container[i]=schedule.getContainer()[i];
        }
    }

    void resize()
    {
        Appointment* old= container;
        capacity*=2;
        container = new Appointment[capacity];
        for(int i = 0; i<size; i++)
        {
            container[i]=old[i];
        }
        delete[] old;

    }



    void swap (Appointment *first, Appointment *second)
    {
        Appointment temp =*first;
        *first = *second;
        *second = temp;

    }




};

void print(const Time*  times, int n)
{
    for(int i = 0; i<n; i++)
    {
        std::cout<<"OBJECT "<< i+ 1 <<": "<<times[i]<<std::endl;

    }
}

void test()
{
    std::cout<<"TEST"<<std::endl
             <<"------------------------------------------------------------------------------------------"<<std::endl
             <<"In this function we will demonstrate how all of the objects and their functions work."<<std::endl
             <<"First lets create a few(5) objects of type Time"<<std::endl<<std::endl;
    Time* times = new Time[5];
    for(int i = 0; i<5; i++)
    {
        std::cout<<"OBJECT "<< i+ 1 <<std::endl;
        std::cin>>times[i];
    }
    std::cout<<std::endl<<"And now lets see if we succeeded"<<std::endl
             <<"(keep in mind that if you input incorrect data, the constructor will automatically correct it adding the extra minutes to the hours and removing the extra hours)"<<std::endl<<std::endl;

    print(times,5);

    std::cout<<std::endl<<"NOW LETS TEST THE OPERATORS"<<std::endl<<std::endl;
    std::cout<<"Lets start with operator +"<<std::endl
             <<"Which of the previously added objects do you want to manipulate?"<<std::endl<<"(input number 1-5): ";
    int index, minutes;
    std::cin>>index;
    std::cout<<"How many minutes do you want to add?";
    std::cin>>minutes;

    std::cout<<std::endl<<"Here are the results"<<std::endl
             <<"from the postfix operation: "<< times[index-1] + minutes<<std::endl
             <<"and the prefix operation: "<< minutes + times[index-1]<<std::endl
             <<std::endl<<"Now lets try summing two objects of type Time"<<std::endl
             <<"Here are the current objects: "<<std::endl;
    print(times, 5);

    std::cout<<std::endl<<"choose two indexes (1-5)";
    int index2;
    std::cin>>index>>index2;

    std::cout<<std::endl<<"The result is: "<<times[index-1] + times [index2 - 1]<<std::endl<<std::endl
             <<"Now lets demonstrate how the ++ operator (adds 1 minute) works on the first object"<<std::endl
             <<"before: "<<times[0]<<std::endl;
    times[0]++;
    std::cout<<"after: "<<times[0]<<std::endl<<std::endl;
    std::cout<<"The difference in minutes between the second and the third object is: "
             <<times[1]<<" - "<<times[2]<<" = "<<times[1]-times[2]<<std::endl;
    std::cout<<"The objects now are:"<<std::endl;
    print(times,5);
    times[4]+=15;
    times[2].reset();
    std::cout<<std::endl<<"After adding 15 minutes to the last objects and resetting the third:"<<std::endl;
    print(times,5);
    std::cout<<std::endl<<"Now lets see if the first object ("<<times[0]<<") <= fourth object ("<<times[3]<<")"<<std::endl
             <<"result: "<<(times[0]<=times[3])<<std::endl;


    std::cout<<std::endl<<"Creating object of type Appointment with name of the person Pesho and time object 1"<<std::endl;
    Appointment ap1(times[0],"Pesho");
    Time t(22,30);
    Appointment ap2(t,"Ivan");
    t.setHours(22);
    t.setMinutes(55);
    Appointment ap3(t,"Valq");
    t.setHours(16);
    t.setMinutes(45);
    Appointment ap4(t,"Mitko");
    t.setHours(20);
    t.setMinutes(10);
    Appointment ap5(t,"Yana");
    std::cout<<ap1<<std::endl;
    std::cout<<std::endl<<"Adding that object and a few others to another object of type Schedule"<<std::endl;
    Schedule schedule(7);
    schedule.addAppointment(ap1);
    schedule.addAppointment(ap2);
    schedule.addAppointment(ap3);
    schedule.addAppointment(ap4);
    schedule.addAppointment(ap5);
    std::cout<<"The schedule is:"<<std::endl<<schedule<<std::endl;
    Schedule copy(schedule);
    copy.clearBetween(Time(22,0),Time(23,0));
    std::cout<<"This is what the schedule will look like if we deleted the appointments between 22:00 and 23:00:"<<std::endl<<copy<<std::endl;
    copy=schedule;
    copy.clear();
    std::cout<<"And this is what the schedule looks like if we clear the appointments within 30 min of each other:"<<std::endl<<copy<<std::endl;
    copy = schedule;
    copy.sort();
    std::cout<<"And this is the schedule sorted:"<<std::endl<<copy<<std::endl;

}

int main()
{
    test();

    return 0;
}
