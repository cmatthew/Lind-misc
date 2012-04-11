#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#define BUFSIZE 8096
#define ERROR 42
#define SORRY 43
#define LOG   44

struct {
	char *ext;
	char *filetype;
} extensions [] = {
	{"gif", "image/gif" },  
	{"jpg", "image/jpeg"}, 
	{"jpeg","image/jpeg"},
	{"png", "image/png" },  
	{"zip", "image/zip" },  
	{"gz",  "image/gz"  },  
	{"tar", "image/tar" },  
	{"htm", "text/html" },  
	{"html","text/html" },  
	{0,0} };

void nweb_log(int type, char *s1, char *s2, int num)
{
	int fd ;
	char logbuffer[BUFSIZE*2];

	switch (type) {
	case ERROR: (void)sprintf(logbuffer,"ERROR: %s:%s Errno=%d exiting pid=%d",s1, s2, errno,getpid()); break;
	case SORRY: 
		assert(sprintf(logbuffer, "<HTML><BODY><H1>nweb Web Server Sorry: %s %s</H1></BODY></HTML>\r\n", s1, s2) > 0);
		assert(write(num,logbuffer,strlen(logbuffer)) != -1);
		assert(sprintf(logbuffer,"SORRY: %s:%s",s1, s2) > 0); 
		break;
	case LOG: 
        assert(sprintf(logbuffer," INFO: %s:%s:%d",s1, s2,num) > 0); 
        break;
	}	
	/* no checks here, nothing can be done a failure anyway */
	if((fd = open("nweb.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
		assert(write(fd,logbuffer,strlen(logbuffer)) != -1); 
		assert(write(fd,"\n",1) != -1);      
		assert(close(fd) == 0);
	}
	if(type == ERROR || type == SORRY) exit(3);
}

/* this is a child web server process, so we can exit on errors */
void web(int fd, int hit)
{
	int j, file_fd, buflen, len;
	long i, ret;
	char * fstr;
    
	static char buffer[BUFSIZE+1]; 
    memset(&buffer, 0, BUFSIZE + 1);
	ret =read(fd, buffer, BUFSIZE); 	/* read Web request in one go */
	if(ret == 0 || ret == -1) {	/* read failure stop now */
		nweb_log(SORRY,"failed to read browser request","",fd);
	}
	if(ret > 0 && ret < BUFSIZE)	/* return code is valid chars */
		buffer[ret]=0;		/* terminate the buffer */
	else buffer[0]=0;

	for(i=0;i<ret;i++)	/* remove CF and LF characters */
		if(buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i]='*';
	nweb_log(LOG,"request",buffer,hit);

	if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) )
		nweb_log(SORRY,"Only simple GET operation supported",buffer,fd);

	for(i=4;i<BUFSIZE;i++) { /* null terminate after the second space to ignore extra stuff */
		if(buffer[i] == ' ') { /* string is "GET URL " +lots of other stuff */
			buffer[i] = 0;
			break;
		}
	}

	for(j=0;j<i-1;j++) 	/* check for illegal parent directory use .. */
		if(buffer[j] == '.' && buffer[j+1] == '.')
			nweb_log(SORRY,"Parent directory (..) path names not supported",buffer,fd);

	if( !strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],"get /\0",6) ) /* convert no filename to index file */
		(void)strcpy(buffer,"GET /index.html");

	/* work out the file type and check we support it */
	buflen=strlen(buffer);
	fstr = (char *)0;
	for(i=0;extensions[i].ext != 0;i++) {
		len = strlen(extensions[i].ext);
		if( !strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
			fstr =extensions[i].filetype;
			break;
		}
	}
	if(fstr == 0) nweb_log(SORRY,"file extension type not supported",buffer,fd);

	if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) /* open the file for reading */
		nweb_log(SORRY, "failed to open file",&buffer[5],fd);

	nweb_log(LOG,"SEND",&buffer[5],hit);
    int size = lseek(file_fd, 0L, SEEK_END);
    assert(lseek(file_fd, 0L, SEEK_SET)==0);
	assert(sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", fstr, size)>0);
	assert(write(fd,buffer,strlen(buffer))!=-1);

	/* send file in 8KB block - last block may be smaller */
	while (	(ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {
		assert(write(fd,buffer,ret) == ret);
	}
    shutdown(fd, SHUT_RDWR);
}


int main(int argc, char **argv)
{
	int i, port, listenfd, socketfd, hit;
	socklen_t length;
	static struct sockaddr_in cli_addr; /* static = initialised to zeros */
	static struct sockaddr_in serv_addr; /* static = initialised to zeros */

	if( argc < 3  || argc > 3 || !strcmp(argv[1], "-?") ) {
		(void)printf("hint: nweb Port-Number Top-Directory\n\n"
	"\tnweb is a small and very safe mini web server\n"
	"\tnweb only servers out file/web pages with extensions named below\n"
	"\t and only from the named directory or its sub-directories.\n"
	"\tThere is no fancy features = safe and secure.\n\n"
	"\tExample: nweb 8181 /home/nwebdir &\n\n"
	"\tOnly Supports:");
		for(i=0;extensions[i].ext != 0;i++)
			(void)printf(" %s",extensions[i].ext);

		(void)printf("\n\tNot Supported: URLs including \"..\", Java, Javascript, CGI\n"
	"\tNot Supported: directories / /etc /bin /lib /tmp /usr /dev /sbin \n"
	"\tNo warranty given or implied\n\tNigel Griffiths nag@uk.ibm.com\n"
		    );
		exit(0);
	}
	if(chdir(argv[2]) == -1){ 
		(void)printf("ERROR: Can't Change to directory %s\n",argv[2]);
		exit(4);
	}

	nweb_log(LOG,"nweb starting",argv[1],getpid());
	/* setup the network socket */
	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)
		nweb_log(ERROR, "system call","socket",0);
	port = atoi(argv[1]);
	if(port < 0 || port >60000)
		nweb_log(ERROR,"Invalid port number (try 1->60000)",argv[1],0);
	serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
 	serv_addr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0)
		nweb_log(ERROR,"system call","bind",0);
	if( listen(listenfd,64) <0)
		nweb_log(ERROR,"system call","listen",0);

	for(hit=1; ;hit++) {
		length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0) {
			nweb_log(ERROR,"system call","accept",0);
        }
        
  
        web(socketfd,hit); /* never returns */
        
    }
}
