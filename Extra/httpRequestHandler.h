#ifndef _HTTPREQUESTHANDLER_H
#define _HTTPREQUESTHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Funcion: request
 * -----------------
 * Procedimiento el cual procesa la consulta del webServer y retorna una respuesta
 * utilizando el protocolo HTTP
 *
 * requestMsg: Mensaje siguiendo el formato HTTP para la consulta
 * currentPath: Direccion donde se decea iniciar el servidor web
 *
 * Retorno:
 * 			Mensaje siguiendo el formato HTTP
*/
char *request(char *requestMsg, char *currentPath);


/*
 * Funcion: getMethod
 * -------------------------
 * Procedimiento por defecto para las consultas bajo el metodo GET,
 * bucara el archivo asociado a la direccion del path, retornando 
 * su contenido en texto plano
 *
 * path: Direccion donde se encuentra el recurso 
 * paramCount: Numero de parametros
 * parameters: Parametros
 * body: mensaje asociado al request
 * 
 * Retorno:
 * 			Mensaje siguiendo el formato HTTP
 */
char *getMethod(char *path, int paramCount, char *parameters[], char *body);

/*
 * Funcion: postMethod
 * -------------------------
 * Procedimiento el cual se encarga de ejecutar codigo en el servidor,
 * pasando como parametro lo que se encuentre en el body
 *
 * path: Direccion donde se encuentra el recurso 
 * paramCount: Numero de parametros
 * parameters: Parametros
 * body: mensaje asociado al request
 * 
 * Retorno:
 * 			Mensaje siguiendo el formato HTTP
 */
char *postMethod(char *path, int paramCount, char *parameters[], char *body);

#endif