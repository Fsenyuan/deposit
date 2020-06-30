#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
int dayofmon[20] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };
class Date {
private:
    int year, month, day;
    int totaldays;
public:
    Date(int y, int m, int d);
    int getyear()const
    {
        return year;
    }
    int getmon()const { return month; }
    int getday()const { return day; }
    int getmaxday()const;
    bool isleapyear()const
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
    void show()const;
    int distance(const Date& date)const
    {
        return abs(totaldays - date.totaldays);
    }
};
int Date::getmaxday()const
{
    if (isleapyear() && month == 2)
    {
        return 29;
    }
    else
    {
        return dayofmon[month] - dayofmon[month - 1];
    }
}

Date::Date(int year, int month, int day) :year(year), month(month), day(day)
{
    if (day <= 0 || day > getmaxday())
    {
        cout << "Invalid date:";
        show();
        cout << endl;
        exit(1);
    }
    int years = year - 1;
    totaldays = years * 365 + years / 4 - years / 100 + years / 400 + dayofmon[month - 1] + day;
    if (isleapyear() && month > 2)totaldays++;

}
void Date::show()const
{
    cout << getyear() << "-" << getmon() << "-" << getday();
}
#endif // DATE_H_INCLUDED

