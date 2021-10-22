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

    /*Recibe un valor y lo agrega al valor actual del
    resultado. Esta operacion es atomica.*/
    virtual void update_value(u_int16_t new_value) override;

    /*Imprime el valor del resultado*/
    virtual void print() override;
};

class Result_mean: public Result{
    private:
    u_int16_t value;
    int rows;
    std::mutex m;
    public:
    explicit Result_mean(int total_rows);
    /*Recibe un valor y lo agrega al valor actual del
    resultado. Esta operacion es atomica.*/
    virtual void update_value(u_int16_t new_value) override;
    /*Imprime el resultado con el formato "suma/total de filas"*/
    virtual void print() override;
};

class Result_max: public Result{
    private:
    u_int16_t value;
    std::mutex m;
    public:
    Result_max();
    /*Recibe un valor y lo compara con el valor actual del
    resultado. Si el nuevo valor es mayor, el valor del resultado
    pasa a ser ese. Esta operacion es atomica.*/
    virtual void update_value(u_int16_t new_value) override;
    /*Imprime el valor del resultado*/
    virtual void print() override;
};

class Result_min: public Result{
    private:
    u_int16_t value;
    std::mutex m;
    public:
    Result_min();
    /*Recibe un valor y lo compara con el valor actual del
    resultado. Si el nuevo valor es menor, el valor del resultado
    pasa a ser ese. Esta operacion es atomica.*/
    virtual void update_value(u_int16_t new_value) override;
    /*Imprime el valor del resultado*/
    virtual void print() override;
};

#endif
