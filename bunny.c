#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
void password(int,char **);//password
void printscreen();//bunny print on screen
char** enterargs(int *);//Line entry
int heavyparse(int,char **); //Main demulitplex
void forkexecvp(int,char **);//main exec
int tofile(int, char**);//redirect to file
int fromfile(int,char**);//redirect from file
int hidden(int, char**);//hidden function
int fromonetoanother(int,char**); //from function to function
int lightparse(int,char**);
char* pwd();
void getmydir();
char* mydir();
void changemydir(int, char**); //Changed dir
void killmyproc(); //Kill process
void exitfunc(int, char **); //My exit function
int main( int argc, char *argv[], char *env[] ){
char **args;
char* currdir;
int i=0;
int arc=0;
int *parc=&arc;
int caseoff=0;
int caseon=0;

while(1){//our main infinit loop
//password(argc,argv);
//currdir=pwd(); //i dont know what i wanted with this
//getmydir();

printscreen();
args=enterargs(parc);//return of created array of strings

caseoff=heavyparse(arc,args);//used caseoff because it was easier to control the program

if(caseoff==0){
	continue;
//i solved this in main if the user enters all spaces or just hits enter to just skip next programs steps and goes from te beginnig
}else if(caseoff==1){
	exitfunc(arc,args);

}else if(caseoff==2){//argumenti
	caseon=lightparse(arc,args);
	//printf("\n%d\n",caseon);check
	if(caseon==0){
	forkexecvp(arc,args);
	}
	else if(caseon==1){
	tofile(arc,args);
	}else if(caseon==2){
		fromfile(arc,args);
	}else if(caseon==3){
		fromonetoanother(arc,args);
	}else if(caseon==4){
	    hidden(arc,args);
	
	}else{
		
	}

}else if(caseoff==3){//cd
	changemydir(arc,args);
}else  if(caseoff==4){//kill
	killmyproc();

}





}//end of infinite loop
return 0;
}
void password(int argc, char **argv){//provjera lozinke
char entry[20];
if(argc>2){
	printf("\nBunny has detected: %d arguments. Bunny needs only 1 argument.\n",argc-1);
	printf("Use like: %s %s\n", argv[0],argv[1]);
	exit(-1);
}
if(argc==1){
	printf("\nThe bunny will let you pass if you know the meaning of life: ");
	scanf("%s", entry);
	if(strcmp("42",entry)==0){
		printf("\nSeems you are worthy!\n");
		return;
		
	}else{
		printf("\nThe bunny hates cheaters!\n");
		exit(-1);
	}
}else if(argc>1){
if(strcmp("42",argv[1])==0){
	printf("\nYou knew it from the begining!\n");
	return;
}else{
	printf("\nThe bunny hates cheaters!\n");
	exit(-1);
}
}
}//end of pass

void printscreen(){
	//simple color yellow
	printf("\e[33m(\\_/)\n");
	
	printf("\e[33m(o.o)    The bunny is expecting a job!\n");
	
	printf("\e[33m(___)0\e[39m");
	printf("\e[33m ");
	getmydir();
	printf(" >> \e[39m");
}//end of print
char** enterargs(int *arc){
	int i=0;//using 3 counters because....reasons.
	int j=0;
	int k=0;
	int n=0;
	int spaceflag=1;//logic expects spaces
	int counter=0;
	char buffer[64];
    char *b = buffer;
    size_t bufsize = 64;
    size_t characters;
	char **argv;

    //printf("Type something: ");
    characters = getline(&b,&bufsize,stdin);
    //printf("%zu characters were read.\n",characters);
   // printf("You typed: '%s'\n",buffer);
    
	
	while(buffer[i]!='\n'){//adding up spaces
		if(buffer[i]!=' '){
			spaceflag=0;
		}
		if(buffer[i]==' ' && spaceflag==0){
		n++;
		spaceflag=1;
		}
		i++;
	}
	if(buffer[characters-2]==' '){//if there are spaces at the end ignore them
		n=n-1;
	}
	if(n==-1){//really important for people who like hitting enter or spaces so much
		n=n+1;}
	//printf("\nThere is %d spaces\n",n);//so there is n+1 arguments, lets load them
	//load
	argv = (char**)malloc(sizeof(char*)*(n+1));
	for(j=0;j<(n+1);j++){
		argv[j] = (char*)malloc(sizeof(char)*32);//there are no arguments this big so i am not fixing this little bug
	}
    //created string table
	//lets fill it
	spaceflag=0;
	i=0;
	j=0;
	while(buffer[i]==' '){//come to the 1st string
		i++;
	}
	while(buffer[i]!='\n'){
		//the insane logic begins
		if(buffer[i]!=' '){
			if(spaceflag==1){
				spaceflag=0;
				j++;
				k=0;
			}
			argv[j][k]=buffer[i];
			k++;
			i++;
		}
		if(buffer[i]==' '){
			i++;
			spaceflag=1;
			
		}
		
		
	}//works
	/*for(i=0;i<n+1;i++){                 //check - maybe gonna need it sometime later
		printf("\nParsed:'%s'",argv[i]);
	}
	printf("\n");*/
	//this has a bug of creating \0 empty double pointer but it was usable in case of "enter" input
	*arc=n+1;
	return argv;

}
int heavyparse(int argc,char **argv){
	int i=0;
	
	//ok its time to divide it on cases and subcases
	//first to make exit and exit function so we can exit the "while" in main
	if(strcmp("exit",argv[0])==0){ // i do expect from user to enter this condition in all lower case letters 
	//it ignores other arguments behind for a reasons
	//return 1; //1- for exit
	 exitfunc(argc, argv);
	 return -1; //just in case of certain errors....
	}
	if(strcmp("",argv[0])==0){//return 0 for no entry
		return 0;
	}else if(strcmp("cd",argv[0])==0){
		return 3;//no brain case of wanting cd
	}else if(strcmp("kill",argv[0])==0){
		return 4;//no brain case of wanting kill
	}else{
		return 2;//anything else, any other input, call for file function is 2 and goes to forkexecvp
	}
	
}

void exitfunc(int argc, char **argv){
	int i=0;
	printf("\n");
	printf("\e[33m(\\(\\\n");
	
	printf("\e[33m( -.-)    The bunny got tired of you!\n");
	
	printf("\e[33mo_(\")(\")\e[39m\n");
	
	
	if(argc >1){
	printf("\n\e[33mThe bunny is lazy...");
	printf("\n\e[33mHe left this arguments for later: \e[39m");
	
	for(i=1;i<argc;i++){// if you have arguments after exit
		printf("\e[31m%s\e[39m",argv[i]);
		if(i<(argc-1)){
		printf(" ");
		}
	}
	printf("\n");
	
	}
	
	exit(0);//exits the complete program
}
void forkexecvp(int argc, char**argv){//main exec with no attached things
	int i;
	int status;
	int pid, cpid;
	  
	  
	  
	  //FORK
	  cpid = fork();
  if (cpid == -1) {              // Error
    perror("fork");
   
  } else if (cpid != 0) {        // Parent
   
   waitpid(cpid, &status, 0);
    /* Provjera izlaznog statusa CHILD procesa */
    if (WIFEXITED(status)) {
      //printf("Izlazni status CHILD procesa: %d\n", WEXITSTATUS(status)); Don't need this.
      if(WEXITSTATUS(status)==127){
		  printf("\e[33mBunny doesn't accept this arguments!\e[39m\n");
	  }
	  fflush(stdout);
    } else if (WIFSIGNALED(status)) {
      fprintf(stderr, "Bunny killed child with: %d\n", \
	      WTERMSIG(status));
      fflush(stderr);
    }
   
   
  } else {                     //Child
    execvp(argv[0], &argv[0]);
	exit(127); // Will never happen
  }
	
	
}
int lightparse(int argc, char**argv){
	int i=0;
	int n=0;
	for(i=0;i<argc;i++){//self explanitory
		if(strcmp(">",argv[i])==0){//1->
			n=1;break;
		}else if(strcmp("<",argv[i])==0){//2-<
			n=2;break;
		}else if(strcmp("|",argv[i])==0){//3-|
			n=3;break;
		}else if(strcmp("&",argv[i])==0){//4-&
			n=4;break;
		}else{
			n=0;      //0-normal
		}
		
	}
	return n;
}
char * pwd(){//current directory
	
	int i;
	int status;
	int pid, cpid;
	char *pwd="pwd";
	char *argv[]={"pwd",NULL};
	
	  
	  
	  
	  //FORK
	  cpid = fork();
  if (cpid == -1) {              // Error
    perror("fork");
   
  } else if (cpid != 0) {        // Parent
   
   waitpid(cpid, &status, 0);
    /* Provjera izlaznog statusa CHILD procesa */
    if (WIFEXITED(status)) {
      //printf("Izlazni status CHILD procesa: %d\n", WEXITSTATUS(status)); Don't need this.
      if(WEXITSTATUS(status)==127){
		  printf("\e[33mBunny doesn't accept this arguments!\e[39m\n");
	  }
	  fflush(stdout);
    } else if (WIFSIGNALED(status)) {
      fprintf(stderr, "Bunny killed child with: %d\n", \
	      WTERMSIG(status));
      fflush(stderr);
    }
   
   
  } else {                     //Child
    execvp("pwd",argv);
	exit(127); // Will never happen
  }
	
 
 return pwd;
	
	
}
void getmydir(){
	 char cwd[1024];
   if (getcwd(cwd, sizeof(cwd)) != NULL){
       fprintf(stdout," %s", cwd);
   }else{
   perror("getcwd() error");}
  
	
	
}
void changemydir(int argc, char**argv){
	char *temp;
    int er;
	if(argc!=2){//cd works only with 2 args for my knowledge
		printf("\n\e[33mWrong number of arguments!!Bunny won\'t change places\e[39m\n");
		return;
	}
	if(strcmp(argv[1],".")==0){
		//pretend nothing is happening and it will all be alright!
		printf("\n\e[33mBunny will stay put!!Bunny won\'t change places!\e[39m\n");
		return;
	}else if(strcmp(argv[1],"..")==0){
        printf("\n\e[33mBunny will climb up!\e[39m\n");
	er=chdir("..");
	}else{
		er=chdir(argv[1]);
	}//basic stuff
	if(er==(-1)){
		printf("\n\e[33mPlace doesn't exist!!Bunny won\'t change places!\e[39m\n");
	}
	
}
char* mydir(){
	char cwd[1024];
	char *dir=cwd;
   if (getcwd(cwd, sizeof(cwd)) != NULL){
       return dir;
   }else{
   perror("getcwd() error");}
}
void killmyproc(){// i am not going to make every option possible, you can kill processes and thats enough!
	int status=0;
	char command='a';
	int target=0;
	char s[32];
	pid_t pid;
	printf("\e[31mBUNNY WILL TASTE BLOOD!\e[39m\n");
	printf("\n\e[31m\e[39m");
	while(1){
	printf("\n\e[31mWHO DO YOU WANT BUNNY TO KILL? everybody->everybody\e[39m\n");
	printf("\e[31m                               process->PID\e[39m\n");
	printf("\e[31m                               exit->exit\e[39m\n");
	printf("\e[31mYOUR COMMAND: \e[39m");
	
	scanf("%s",&s);
	getchar();//this bug....so simple so nasty
	//printf("\n%s\n",s); //got some nasty bugs here so i needed to check
	target=atoi(s);
	pid=target;
	//printf("\n%d\n",target);
	if(strcmp("exit",s)==0){
		break;
	}
	if(strcmp("everybody",s)==0){
		target=0;
	}
	if(target==0){
		printf("\e[31mBUNNY KILLED EVERYBODY INCLUDING HIM SELF!\e[39m\n");
		kill(0,9); //exits ./bunny no return
	}else{
	status=kill(pid,9);
	}
	
	
	if(status==(-1)){//basic stuff
		printf("\n\e[31mBUNNY DIDN'T KILL THE TARGET! HE MISSED!\e[39m\n");
	}
	
	}
}
int tofile(int argc,char** argv){
	//ok lets make this pretty
	int i;
	int status;
	int pid, cpid;
	int fd;//redirection
	char *ime_dat;
	int count=0;
	char **args;
	int argcnt=argc-2;
	if((argc<3)||(strcmp(argv[argc-2],">")!=0)){
		printf("\e[33mBunny detected wrong form!\e[39m\n");
		return 0;
	}
	ime_dat=argv[argc-1];
	//printf("\n%s\n",ime_dat);
	args=(char**)malloc(sizeof(char*)*argcnt);//not needed but k
    for (i = 0; i < argcnt; ++i){
    args[i] = (char*)malloc(sizeof(char)*64);}
	for(i = 0; i < argcnt; ++i){
		args[i]=argv[i];
		
	}
	//redirection
	
     
	  
	  
	  
	  //FORK
	  cpid = fork();
  if (cpid == -1) {              // Error
    perror("fork");
   
  } else if (cpid != 0) {        // Parent
   
   waitpid(cpid, &status, 0);
    /* Provjera izlaznog statusa CHILD procesa */
    if (WIFEXITED(status)) {
      //printf("Izlazni status CHILD procesa: %d\n", WEXITSTATUS(status)); Don't need this.
	  printf("\n\e[33mBunny ended writing!\e[39m\n");
      if(WEXITSTATUS(status)==127){
		  printf("\e[33mBunny doesn't accept this arguments!\e[39m\n");
	  }
	  fflush(stdout);
    } else if (WIFSIGNALED(status)) {
      fprintf(stderr, "Bunny killed child with: %d\n", \
	      WTERMSIG(status));
      fflush(stderr);
    }
   
   
  } else {                     //Child
   //redirection
   fd=open(ime_dat, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd < 0) {
    perror("open");
    exit(0);
  }

  dup2(fd, STDOUT_FILENO);

  if (fd != STDOUT_FILENO)
    close(fd);
   execvp(args[0], &args[0]);
	exit(127); // Will never happen
  }
	
	
	

	
}
int fromfile(int argc,char** argv){
	//ok lets make this pretty
	int i;
	int status;
	int pid, cpid;
	int fd;//redirection
	char *ime_dat;
	int count=0;
	char **args;
	int argcnt=argc-2;
	if((argc<3)||(strcmp(argv[argc-2],"<")!=0)){
		printf("\e[33mBunny detected wrong form!\e[39m\n");
		return 0;
	}
	ime_dat=argv[argc-1];
	//printf("\n%s\n",ime_dat);
	args=(char**)malloc(sizeof(char*)*argcnt);//not needed but k
    for (i = 0; i < argcnt; ++i){
    args[i] = (char*)malloc(sizeof(char)*64);}
	for(i = 0; i < argcnt; ++i){
		args[i]=argv[i];
		
	}
	//redirection
	
     
	  
	  
	  
	  //FORK
	  cpid = fork();
  if (cpid == -1) {              // Error
    perror("fork");
   
  } else if (cpid != 0) {        // Parent
   
   waitpid(cpid, &status, 0);
    /* Provjera izlaznog statusa CHILD procesa */
    if (WIFEXITED(status)) {
      //printf("Izlazni status CHILD procesa: %d\n", WEXITSTATUS(status)); Don't need this.
	  printf("\n\e[33mBunny ended writing!\e[39m\n");
      if(WEXITSTATUS(status)==127){
		  printf("\e[33mBunny doesn't accept this arguments!\e[39m\n");
	  }
	  fflush(stdout);
    } else if (WIFSIGNALED(status)) {
      fprintf(stderr, "Bunny killed child with: %d\n", \
	      WTERMSIG(status));
      fflush(stderr);
    }
   
   
  } else {                     //Child
   //redirection
   fd=open(ime_dat,O_RDONLY);
  if (fd < 0) {
    perror("open");
    exit(0);
  }

  dup2(fd, STDIN_FILENO);

  if (fd != STDIN_FILENO)
    close(fd);
  
  execvp(args[0], &args[0]);
	exit(127); // Will never happen
  }
}
int fromonetoanother(int argc,char** argv){
		//ok lets make this pretty
	int i;
	int pid;
	int cpid;
	int status;
	int pipefd[2];
	int count=0;
	int argcnt=0;
	int argcnt2=0;
	int middle=0;
	char **args;
	char **args2;
	for(i=0;i<argc;i++){
	if(strcmp(argv[i],"|")==0){
	  middle=i;
	}
	}
	argcnt=middle;
	argcnt2=argc-(middle +1);
	//args2=&argv[middle+1];
	args=(char**)malloc(sizeof(char*)*argcnt);//not needed but k
    for (i = 0; i < argcnt; ++i){
    args[i] = (char*)malloc(sizeof(char)*64);}
	for(i = 0; i < argcnt; ++i){
		args[i]=argv[i];
		
	}
		args2=(char**)malloc(sizeof(char*)*argcnt2);//not needed but k
    for (i = 0; i < argcnt2; ++i){
    args2[i] = (char*)malloc(sizeof(char)*64);}
	for(i = 0; i < argcnt2; ++i){
		args2[i]=argv[middle+1 + i];
		
	}
	//forkexecvp(argcnt,args);

	//forkexecvp(argcnt2,args2);
	/*	for(i = 0; i < argcnt; ++i){
		printf("\n1: %s\n",args[i]);
		
	}
		for(i = 0; i < argcnt; ++i){
		printf("\n2: %s\n",args2[i]);
		
	}*/
	
  pipe(pipefd);
	
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
		  cpid = fork();
  if (cpid == -1) {              // Error
    perror("fork");
   
  } else if (cpid != 0) {        // Parent
   
   waitpid(cpid, &status, 0);
    /* Provjera izlaznog statusa CHILD procesa */
    if (WIFEXITED(status)) {
      //printf("Izlazni status CHILD procesa: %d\n", WEXITSTATUS(status)); Don't need this.
	  printf("\n\e[33mBunny ended writing!\e[39m\n");
      if(WEXITSTATUS(status)==127){
		  printf("\e[33mBunny doesn't accept this arguments!\e[39m\n");
	  }
	  fflush(stdout);
    } else if (WIFSIGNALED(status)) {
      fprintf(stderr, "Bunny killed child with: %d\n", \
	      WTERMSIG(status));
      fflush(stderr);
    }
   
   
  } else {                     //Child
//////////////////////////////////////////////////////
	  pid = fork();
  if (pid == -1) {              // Error
    perror("fork");
   
  } else if (pid != 0) {        // Parent
       

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute cat

      execvp(args[0], &args[0]);
    
 
  } else {                     //Child
     // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      dup2(pipefd[0], 0);

      // close unused hald of pipe

      close(pipefd[1]);

      // execute grep

      execvp(args2[0], &args2[0]);
    }
  
  }
	
	
	
}
int hidden(int argc,char** argv){
char ** args;
int i=0;
if(strcmp(argv[argc-1],"&")!=0){
printf("\e[33mBunny detected wrong form!\e[39m\n");
		return 0;
	}
args=(char**)malloc(sizeof(char*)*(argc-1));//not needed but k
    for (i = 0; i < (argc-1); ++i){
    args[i] = (char*)malloc(sizeof(char)*64);}
	for(i = 0; i < (argc-1); ++i){
		args[i]=argv[i];
		
	}
forkexecvp(argc-1,args);








}


