#include <string.h>
#include "info.h"

Product createProduct(char p_name[BUF], int p_price, int cod_produto){
    Product p;
    strncpy(p.p_name,p_name,BUF);
    p.p_price = p_price;
    p.cod_prod = cod_produto;
    return p;

}
Request createRequest(int cod_produto, int num_proc){
    Request r;
    r.num_processo = num_proc;
    r.cod_prod = cod_produto;
    return r;
}