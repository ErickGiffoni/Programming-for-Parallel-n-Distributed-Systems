/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _MAIN_H_RPCGEN
#define _MAIN_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct operandos {
	int *x;
	int n;
};
typedef struct operandos operandos;

#define MAIN 101
#define UM 100

#if defined(__STDC__) || defined(__cplusplus)
#define somavet 1
extern  int * somavet_100(operandos *, CLIENT *);
extern  int * somavet_100_svc(operandos *, struct svc_req *);
extern int main_100_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define somavet 1
extern  int * somavet_100();
extern  int * somavet_100_svc();
extern int main_100_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_operandos (XDR *, operandos*);

#else /* K&R C */
extern bool_t xdr_operandos ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_MAIN_H_RPCGEN */
