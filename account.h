#ifndef ACCOUNT_H_INCLUDED
#define ACCOUNT_H_INCLUDED
#include "date.h"
#include "accumulator.h"
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
string to_string(int x)
{
    string s;
    if (x == 0) s += '0';
    while (x)
    {
        s += (x % 10) + '0';
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

class Account
{
private:
    string id;
    double balance;
    static double total1, total2;
    vector<string>rec;
protected:
    Account(const Date& date, const string& id);
    void record(const Date& date, double amount, const string& desc);
    void error(const string& msg)const;
public:
    const string& getid()
    {
        return id;
    }
    void changetotal1(double amount)
    {
        total1 += amount;
    }
    void changetotal2(double amount)
    {
        total2 += amount;
    }
    double getbalance()const
    {
        return balance;
    }
    static double gettotal1()
    {
        return total1;
    }
    static double gettotal2()
    {
        return total2;
    }
    void show()const;
};
class SavingAccount :public Account
{
private:
    Accumulator acc;
    double rate[5] = { 0,0.35 * 0.01 / 365,1.5 * 0.01 / 365,2.10 * 0.01 / 365,2.75 * 0.01 / 365 };
    int index;

public:
    SavingAccount(const Date& date, const string& id, int index);
    double getrate(int i)
    {
        return rate[i];
    }
    void changerate(double rate1, int i)
    {
        rate[i] = rate1;
    }
    void changekind(int i)
    {
        index = i;
    }
    void deposit(const Date& date, double amount, const string& desc);
    void withdraw(const Date& date, double amount, const string& desc);
    void settle(const Date& date);
};

class CreditAccount :public Account
{
private:
    Accumulator acc;
    double credit;
    double rate;
    double fee;
    double getDebt()const
    {
        double balance = getbalance();
        return (balance < 0 ? balance : 0);
    }
public:
    CreditAccount(const Date& date, const string& id, double credit, double rate, double fee);
    double getcredit()const
    {
        return credit;
    }
    double getrate()const
    {
        return rate;
    }
    double getfee()const
    {
        return fee;
    }
    double getAvailableCredit()const
    {
        if (getbalance() < 0)
        {
            return credit + getbalance();
        }
        else return credit;
    }
    void deposit(const Date& date, double amount, const string& desc);
    void withdraw(const Date& date, double amount, const string& desc);
    void settle(const Date& date);
    void show()const;
};

double Account::total1 = 0;
double Account::total2 = 0;
Account::Account(const Date& date, const string& id) :id(id), balance(0)
{
    date.show();
    cout << "\t#" << id << " created" << endl;
}
void Account::record(const Date& date, double amount, const string& desc)
{
    amount = floor(amount * 100 + 0.5) / 100;
    balance += amount;
    //total1+=amount;
    date.show();
    cout << "\t#" << id << "\t" << amount << "\t  " << balance << "\t" << desc << endl;
    string t;
    t = t + to_string((float)date.getyear()) + "-" + to_string((float)date.getmon()) + "-" + to_string((float)date.getday()) + ": ";
    t = t + " #" + id + "    " + to_string(amount) + "    " + to_string(balance) + " " + desc;
    rec.push_back(t);
}
void Account::show()const
{
    cout << id << "\tBalance:" << balance << endl;;
    for (int i = 0; i < rec.size(); i++)
    {
        cout << rec[i] << endl;
    }
}
void Account::error(const string& msg)const
{
    cout << "Error(#" << id << "):" << msg << endl;
}

SavingAccount::SavingAccount(const Date& date, const string& id, int index)
    :Account(date, id), index(index), acc(date, 0) {}

void SavingAccount::deposit(const Date& date, double amount, const string& desc)
{
    changetotal1(amount);
    record(date, amount, desc);
    acc.change(date, getbalance());
}
void SavingAccount::withdraw(const Date& date, double amount, const string& desc)
{
    if (amount > getbalance())
    {
        error("not enough money");
    }
    else
    {
        changetotal1(-amount);
        record(date, -amount, desc);
        acc.change(date, getbalance());
    }
}
void SavingAccount::settle(const Date& date)
{
    double interest = acc.getsum(date) * rate[index];
    // cout<<interest<<endl;
    if (interest != 0)
    {
        record(date, interest, "interest");
        changetotal1(interest);
    }
    acc.reset(date, getbalance());
}


CreditAccount::CreditAccount(const Date& date, const string& id, double credit, double rate, double fee)
    :Account(date, id), credit(credit), rate(rate), fee(fee), acc(date, 0) {};
void CreditAccount::deposit(const Date& date, double amount, const string& desc)
{
    changetotal2(-amount);
    record(date, amount, desc);
    acc.change(date, getDebt());
}
void CreditAccount::withdraw(const Date& date, double amount, const string& desc)
{
    if (amount - getbalance() > credit)
    {
        error("not enough credit");
    }
    else
    {
        changetotal2(amount);
        record(date, -amount, desc);
        acc.change(date, getDebt());
    }
}
void CreditAccount::settle(const Date& date)
{
    double interest = acc.getsum(date) * rate;
    if (interest != 0)
    {
        record(date, interest, "interest");
        changetotal2(interest);
    }
    if (date.getmon() == 1)
    {
        record(date, -fee, "annual fee");
        changetotal2(fee);
    }
    acc.reset(date, getDebt());
}
void CreditAccount::show()const
{
    Account::show();
    cout << "\tAvailable credit:" << getAvailableCredit();
}
#endif // ACCOUNT_H_INCLUDED
