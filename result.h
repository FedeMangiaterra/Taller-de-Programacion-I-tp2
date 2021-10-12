#ifndef RESULT_H
#define RESULT_H

#include <stdio.h>
#include <stdlib.h>
#include <mutex>

class Result{
    public:
    virtual void update_value(u_int16_t new_value) = 0;
    virtual void print() = 0;
    virtual ~Result() = 0;
};

class Result_sum: public Result{
    private:
    u_int16_t value;
    std::mutex m;
    public:
    Result_sum();
    virtual void update_value(u_int16_t new_value) override;
    virtual void print() override;
};

class Result_mean: public Result{
    private:
    u_int16_t value;
    int rows;
    std::mutex m;
    public:
    explicit Result_mean(int total_rows);
    virtual void update_value(u_int16_t new_value) override;
    virtual void print() override;
};

class Result_max: public Result{
    private:
    u_int16_t value;
    std::mutex m;
    public:
    Result_max();
    virtual void update_value(u_int16_t new_value) override;
    virtual void print() override;
};

class Result_min: public Result{
    private:
    u_int16_t value;
    std::mutex m;
    public:
    Result_min();
    virtual void update_value(u_int16_t new_value) override;
    virtual void print() override;
};

#endif
