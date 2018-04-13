#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void toto1() {
  sleep(4);
  printf("au revoir\n");
}

void toto2() {
  if(fork()==0) {
    // fils
    sleep(4);
    printf("fini\n");
  } else {
    // pere
    sleep(2);
    printf("bye\n");
  }

  // le processus père termine en affichant en affichant "bye",
  //  et une nouvelle ligne de commande apparait
  // On a l'impression que tout le programme est terminé,
  //  mais 2 secondes plus tard, "fini" est affiché
}

void toto3() {
  if(fork()==0) {
    sleep(4);
    printf("fini\n");
  } else {
    abort();
    // exit(0);
  }
  // seulement le processus père s'arrête, le processus fils continue
  // abort affiche "Aborted"
}

void toto4() {
  pid_t pid = fork();
  if(pid==0) {
    sleep(4);
    printf("fini\n");
  } else {
    sleep(2);
    kill(0, SIGKILL);
    printf("bye\n");
  }
  // "fini" ne s'affiche jamais, le père tue le fils :'(
}

void toto5() {
  if(fork()==0) {
    // char* argv[] = {"toto", "1", NULL};
    // execv("./toto", argv);
    system("./toto 1");
    sleep(4);
    printf("fini\n");
  } else {
    sleep(2);
    printf("bye\n");
  }
  // Avec system(), il fait un appel pour que le systeme lance toto1,
  //  puis le processus continue et affiche quand même "fini".
  // Avec execv(), le processus est remplace par toto1, et affiche
  //  seulement "au revoir".
}



void (*toto[5]) () = {toto1, toto2, toto3, toto4, toto5};

int main(int argc, char** argv) {
  if (argc != 2) {
		fprintf(stderr,
			"Usage: toto NUM, where NUM = question number\n");
		return 1;
	}

	toto[atoi(argv[1])-1]();
  return 0;
}
