#ifndef INFO_H
#define INFO_H

#define BUF 64

// Product Structure

typedef struct
{
    char p_name[BUF];
    int p_price;
    int cod_prod;
} Product;

//Product request
typedef struct
{
    int num_processo;
    int cod_prod;
} Request;

Product createProduct(char p_name[BUF], int p_price, int cod_produto);
Request createRequest(int cod_produto, int num_proc);
#endif