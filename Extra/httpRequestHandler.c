#include "httpRequestHandler.h"

char *NOT_IMPLEMENTED 	= "HTTP/1.1 501 Not Implemented\n";
char *OK 				= "HTTP/1.1 200 OK\n";
char *NOT_FOUND 		= "HTTP/1.1 404 Not Found\n";
char *INTERNAL_ERROR 	= "HTTP/1.1 500 Internal Server Error\n";

/*
 * Funcion: headerCount
 * -------------------
 * Procedimiento el cual cuenta la cantidad de "header lines"
 * mediante un calculo utilizando '\n'
 *
 * requestMsg: Mensaje de entrada utilizando el formato HTTP
 *
 * Retorno:
 * 			Numero de headers
 */
int headerCount(char *requestMsg){

	char c1, c2;
	int count, ith;
	
	c1 = 1;
	c2 = 1;
	count = 0;
	ith = 0;
	while(c1){
		c1 = requestMsg[ith];
		c2 = requestMsg[ith+2];
		if(c1 == '\r'){
			
			if(c1 == c2 || !c2)
				break;

			count++;
		}
		ith++;
	}
	return count;
}

/*
 * Funcion: paramCount
 * -------------------
 * Procedimiento el cual calcula la cantidad de parametros ingresados en el path
 * mediante la cuenta de los caracteres '&' si se presenta el caracter '?' en el
 * path
 *
 * path: Direccion del recurso sin haber sido parseada
 *
 * Retorno:
 * 			Numero de parametros
 */
int paramCount(char *path){

	char c, *s;
	int count, ith;

	count = 0;
	s = strchr(path, '?');
	if(s != NULL){

		c = 1;
		count++;
		ith = 0;
		while(c){
			
			c = s[ith];
			if(c == '&')
				count++;

			ith++;
		}
	}
	return count;
}

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
char *request(char *requestMsg, char *currentPath){

	int pathLen, currentPathLen, paramsCount, headersCount, count, ith;
	char *method, *path, *realPath, **parameters, *httpVersion, **headers, *body, *s;

	headersCount = headerCount(requestMsg);

	method = strtok(requestMsg, " ");
	path = strtok(NULL, " ");
	httpVersion = strtok(NULL, "\r\n");

	if(headersCount > 0){
		headers = (char**)malloc(headersCount * sizeof(char*));
		for(int i = 0; i < headersCount; i++){

			headers[i] = strtok(NULL, "\r\n");
		}
	}
	

	body = strtok(NULL, "\r\n\0");

	paramsCount = paramCount(path); 	//Si el path presenta parametros, aqui se parsean
	if(paramsCount > 0){

		path = strtok(path, "?");
		parameters = (char**)malloc(paramsCount * sizeof(char*));

		count = paramsCount;
		count--;
		ith = 0;
		while(count--){

			parameters[ith] = strtok(NULL, "&");
			ith++;
		}
		parameters[ith] = strtok(NULL, "\0");
	}

	currentPathLen = strlen(currentPath);
	pathLen = strlen(path);
	realPath = malloc((currentPathLen + pathLen) * sizeof(char));
	strcpy(realPath, currentPath);
	strcat(realPath, path+1);

	if(!strcmp(method, "GET")){
		
		return getMethod(realPath, paramsCount, parameters, body);

	}else if(!strcmp(method, "POST")){
		
		return postMethod(realPath, paramsCount, parameters, body);

	}
		
	printf("Metodo <%s> no soportado\n", method);
	return NOT_IMPLEMENTED;
}

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
char *getMethod(char *path, int paramCount, char *parameters[], char *body){
	
	char *response;
	int messageLen;
	long responseLen, fileLen;
	FILE *fp;

	fp = fopen(path, "r");

	if(fp){
		
		messageLen = strlen(OK);

		fseek(fp, 0 , SEEK_END);
		fileLen = ftell(fp);
		rewind(fp);

		responseLen = messageLen + fileLen + 1;

		response = calloc(responseLen, sizeof(char));
		strcpy(response, OK);
		fread(response + messageLen, fileLen, 1, fp);

		fclose(fp);
		return response;
	}
	return NOT_FOUND;
}

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
char *postMethod(char *path, int paramCount, char *parameters[], char *body){
	
	char *call_1 = "cd ";
	char *call_2 = ";./";
	char *call_3 = " ";
	char *call_4 = " > .temp";
	char *program, *systemCall, *tempFile, *response;
	int result, pathLen, systemCallLen, tempFileLen, messageLen, fileLen, responseLen;
	FILE *fp;

	pathLen = strlen(path);

	program = NULL;
	for(int i = pathLen-1; i > 0; i--){
		if(*(path+i) == '/'){
			program = path+i;
			break;
		}
	}

	if(program)
		*program = 0;
	program += 1;
	if(program){

		systemCallLen = strlen(call_1);
		systemCallLen += strlen(call_2);
		systemCallLen += strlen(call_3);
		systemCallLen += strlen(call_4);
		systemCallLen += strlen(path);
		systemCallLen += strlen(program);
		systemCallLen += strlen(body);
		systemCallLen++;

		systemCall = calloc(systemCallLen, sizeof(char));
		strcpy(systemCall, call_1);
		strcat(systemCall, path);
		strcat(systemCall, call_2);
		strcat(systemCall, program);
		strcat(systemCall, call_3);
		strcat(systemCall, body);
		strcat(systemCall, call_4);

		result = system(systemCall);
		if(result)
			return INTERNAL_ERROR;

		tempFileLen = strlen(path);
		tempFileLen += strlen(".temp");
		tempFileLen++;
		tempFile = malloc(tempFileLen * sizeof(char));

		strcpy(tempFile, path);
		strcat(tempFile, "/.temp");
		fp = fopen(tempFile, "r");
		if(fp){
			
			messageLen = strlen(OK);

			fseek(fp, 0 , SEEK_END);
			fileLen = ftell(fp);
			rewind(fp);

			responseLen = messageLen + fileLen + 2;

			response = calloc(responseLen, sizeof(char));
			strcpy(response, OK);
			strcat(response, "\n");
			fread(response + messageLen + 1, fileLen, 1, fp);

			fclose(fp);
			return response;
		}
		return OK;
	}
	return NOT_FOUND;
}

/*for(int i = 0; i < strlen(requestMsg); i++){

		if(requestMsg[i] == 13)
			printf(" 13");

		else if(requestMsg[i] == '\n')
			printf("10\n");

		else
			printf("%c", requestMsg[i]);
	}

	printf("xxxx\n");
	printf("%s\n", method); printf("%s\n", path);
	for(int i = 0; i < paramsCount; i++) printf("%s\n", parameters[i]);
	printf("%s\n--", httpVersion);
	for(int i = 0; i < headersCount; i++)  printf("%s\n", headers[i]);
	if(body)
		printf("--%sxxxx\n", body);
		*/