#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

/// Struct data for user accounts.
typedef struct UserAccount{
    char name[50];
    char surname[50];
    char nickname[50];
    char password[50];
    struct UserAccount *next;
    int score;
}UserAccount;

//// Sort algorithm to sort linked list.
UserAccount *sortLinkedList(UserAccount *head){
    UserAccount *p;
    UserAccount *q;
    UserAccount *r;
    UserAccount *end;
    UserAccount *tmp;
    for(end=NULL;end!=head->next;end=p){
        for(r=p=head;p->next!=end;r=p,p=p->next){
            q=p->next;
            if(p->score<q->score){
                p->next=q->next;
                q->next=p;
                if(p!=head){
                    r->next=q;
                }
                else{
                    head=q;
                }
                tmp=p;
                p=q;
                q=tmp;
            }
        }
    }
    return head;

}

// This functions creates a new user account node and returns its adress;
UserAccount *createAccount(char *username){
     UserAccount *acc=calloc(1,sizeof(UserAccount));
     printf("Enter Your Name : ");
     scanf("%s",acc->name);
     printf("Enter Your Surname : ");
     scanf("%s",acc->surname);
     strcpy(acc->nickname,username);
     printf("Enter Your Password : ");
     scanf("%s",acc->password);
     acc->score=0;

     return acc;
}


// This funciton read a txt file to represent map and returns this map as char matrix.
char  **readLabyrinth(char *File_Name){
    FILE *fp = fopen(File_Name, "r");

    char bufferR[30];
    int i=0;
    int rawsize=0;
    while(!feof(fp)){
        fgets(bufferR, sizeof(bufferR), fp);
        rawsize++;
    }
    bufferR[0]='\0';
    fseek(fp,0,SEEK_SET);
    char **labyrinth = (char **)calloc(rawsize, sizeof(char*));

    while(!feof(fp)){
        if (fgets(bufferR, sizeof(bufferR), fp) != NULL) {
            labyrinth[i] = (char *)calloc((strlen(bufferR)+1), sizeof(char));
            bufferR[strcspn(bufferR, "\n")] = '\0'; // Removing newline if present
            strcpy(labyrinth[i], bufferR); // Copying content to labyrinth
            i++;
        }

    }
    fclose(fp);
    return labyrinth;


}

// This function prints the whole map in a for loop
void printLabyrinth(char**lybrnth){
    int i,j;
    printf("# # # # # # # # # # # # \n");
    for(i=0;i<6;i++){
        printf("#");
        for(j=0;j<10;j++){
            if(lybrnth[i][j]=='X') printf(" \033[1;31m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='1') printf(" \033[1;35m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='p') printf(" \033[1;34m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='P') printf(" \033[1;34m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='e') printf(" \033[1;33m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='E') printf(" \033[1;33m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='0') printf(" \033[1;37m%c\033[0m",lybrnth[i][j]);
            if(lybrnth[i][j]=='K') printf(" \033[1;36m%c\033[0m",lybrnth[i][j]);               
            if(lybrnth[i][j]=='C') printf(" \033[1;32m%c\033[0m",lybrnth[i][j]);       

        }
        printf(" #\n");
    }       
    printf("# # # # # # # # # # # # \n");          
}

//This function read a data binary file , if function can't read the file , creates a new one and returns null;
UserAccount *readUserData(){
    
    FILE *fp = fopen("userdata4.bin","rb");
    if(fp==NULL){
        printf("okuma hatasi");
        fp = fopen("userdata4.bin","wb");
    }
    fseek(fp,0,SEEK_END);
    if((ftell(fp)==0)){
        printf("\n UserData empty!");
        sleep(1);
        system("cls");
        return NULL;
    }

    rewind(fp);
    UserAccount *head=calloc(1,sizeof(UserAccount));
    head->next=NULL;
    UserAccount *prevData=head;
    UserAccount *currData;
    while (fread(prevData,sizeof(UserAccount),1,fp))
    {   
        currData=calloc(1,sizeof(UserAccount));
        prevData->next=currData;
        currData->next=NULL;
        prevData=currData;

    }
    fclose(fp);
    return head;

}

// This function prints the linked list
void printLinkedList(UserAccount *head){

      UserAccount *ptr=head;

      while(ptr!=NULL){
        printf("NAME : %s , Surname : %s , NickName : %s , Password : %s \n",ptr->name,ptr->surname,ptr->nickname,ptr->password);
        ptr=ptr->next;
      } 


}

// This function creates a new user account and it adds to linked list also save the new user account data in appending mode.
UserAccount *updateUserData(char *file_name,UserAccount *head){
    FILE *fp = fopen(file_name,"ab");
    if(!fp){
            perror("\nERROR!");
        }
    UserAccount *ptr=head;
    UserAccount *ptr2=head;
    char username[50];
    int savedUser;
    do{
    system("cls");    
    savedUser=1;
    printf("\nEnter Your Username : ");
    scanf("%s",username);
    ptr2=head;
    while(ptr2!=NULL){
        if(!strcmp(ptr2->nickname,username)){
            savedUser=0;
        }
        ptr2=ptr2->next;
    }
    if(savedUser==0){
        printf("\nThis Username Already Taken, Pick another..");
        sleep(2);
    }
    }while(savedUser==0);
    if(head==NULL){
        head=createAccount(username); // head boşssa yeni data oluştur
        fwrite(head,sizeof(UserAccount),1,fp);
        fclose(fp);
        return head;
    }
    else{
        printf("\nHesap olusturuluyor\n");
        sleep(1);
         while(ptr->next!=NULL){
         ptr=ptr->next;
         }
         ptr->next=createAccount(username);
         ptr->next->next=NULL;
         fwrite(ptr->next,sizeof(UserAccount),1,fp);
         fclose(fp);
         return ptr->next;

    }
}

// This function is for login. It takes the head of linked list and checks usernames and passwords , 
// if login is succesfull it returns the login account.
UserAccount *loginmenu(UserAccount *head){
    UserAccount *loginaccount;
    char username[50];
    char password[50];
    char a;
    int flag=1;
    int savedname=0;
    int savedpassword=0;
    do{
    system("cls");
    printf("**************Welcome to The Game***************\n");
    printf("\t1.LOGIN\n");
    printf("\t2.SIGN UP\n");
    printf("\tYour choice : ");
    scanf(" %c",&a);
    switch (a)
    {
    case '1':
        system("cls");
        do{
        system("cls");
        if(head==NULL){
            printf("\n User Data Empty Please Sign Up");
            sleep(1);
            break;
        }
        printf("\n\tEnter Your Username : ");
        scanf("%s",username);
        UserAccount *ptr=head;
        UserAccount *user;
        while(ptr!=NULL){
            if(!strcmp(username,ptr->nickname)){      
               savedname=1;
               user=ptr;
            }
            ptr=ptr->next;
        }
        if(savedname==1){
            printf("\nUsername found\n");
            sleep(2);
            do{
            system("cls");        
            printf("Enter your password :");
            scanf("%s",password);
            if(!strcmp(password,user->password)){
               savedpassword=1;
               loginaccount=user;
               printf("Successfuly logged in..\n");
            }
            else{
                printf("Invalid Password Try Again ! \n");
                sleep(2);
            }

            }while(savedpassword==0);
            
        }
        else{
            printf("\nUsername not found Try Again!");
            sleep(2);
        }
        }while(savedname==0);
        if(head!=NULL){
            flag=0;
        }
        
        break;

    case '2':
        system("cls");
        printf("signin up...\n");
        loginaccount=updateUserData("userdata4.bin",head);
        printf("succesfuly signed up...\n");

        flag=0;
        break;
        
    
    default:       
        printf("\nInvalid option !!\n");
        sleep(1);
        
    }
    }while(flag==1);

    return loginaccount;

}



// This function for recursion autoplay algorithm . It checks the boundries.
// if the new location out of boundries it returns false;
bool boundryOut(int xraw , int xcol){

    
    if(xraw<0) return false;
    else if(xraw>5) return false;


    else if(xcol<0) return false;
    else if(xcol>9) return false;
    else return true;

}
//This functions updates the user's location in for given coordinates at the map.
void updatePlayerLocation(char **labyrinth,int xraw,int xcol,int xrawOld,int xcolOld){
    labyrinth[xraw][xcol]='X';
}


//This function for autogameplay. It is a recursion algorithm and it adds to stack new positions and if it finds the exit returns 1 on all stack.
//Also each step it checks for particules if there is a particules certain position it adds to collectables array to save collected particules.
//If it goes to wrong way it comebacks the start position and try a different direction
int autoGameplay(char **labyritnh,int xraw,int xcol,char  **labelmatrix, int* coordinates,char *collectables,int collect_size){
    // sınırları kontrol et ! eğer çıktıysa return 0;
    // karadelik ise return 0;
    // visited label 1 ise return 0;
    // çıkış gelirse (C)  return 1;
    if(!boundryOut(xraw,xcol)){
        return 0;
    }
    else if(labyritnh[xraw][xcol]=='K'){
        return 0;
    }
    else if(labelmatrix[xraw][xcol]=='1'){
        return 0;
    }
    else if(labyritnh[xraw][xcol]=='1'){
        return 0;
    }
    else if(labyritnh[xraw][xcol]=='C'){
        
        labelmatrix[xraw][xcol]='1'; //labeling 1 
        int tempx = coordinates[0];
        int tempy = coordinates[1];
        labyritnh[tempx][tempy] = '0';//making 0 past location

        labyritnh[xraw][xcol] = 'X';
        system("cls");
        printLabyrinth(labyritnh);
        printf("\n----------Exit found!!!");
        sleep(2);
        return 1;
        
    }
    
    


    system("cls");
    printLabyrinth(labyritnh);
    int i=0;
        printf("\nCollected Particules : ");
        while(collectables[i]!='\0'){
            printf("%c,",collectables[i]); //printing collected particules en each step
            i++;
        }
    sleep(1);
    labelmatrix[xraw][xcol]='1'; //labeling positions as 1 
    int tempx = coordinates[0];
    int tempy = coordinates[1];
    labyritnh[tempx][tempy] = '0'; // making 0 past location
    if(labyritnh[xraw][xcol]!='0'&&labyritnh[xraw][xcol]!='X'){
        collectables[collect_size]=labyritnh[xraw][xcol]; // array for particules
        collect_size++;

    }
    labyritnh[xraw][xcol] = 'X';
    coordinates[0] = xraw;
    coordinates[1] = xcol;



    if(autoGameplay(labyritnh,xraw-1,xcol,labelmatrix,coordinates,collectables,collect_size)){
        return 1;
    }
    if(autoGameplay(labyritnh,xraw,xcol+1,labelmatrix,coordinates,collectables,collect_size)){
        return 1;
    }
    if(autoGameplay(labyritnh,xraw+1,xcol,labelmatrix,coordinates,collectables,collect_size)){
        return 1;
    }
    if (autoGameplay(labyritnh,xraw,xcol-1,labelmatrix,coordinates,collectables,collect_size)){
        return 1;
    }



    labelmatrix[xraw][xcol]='0';

    return 0;

}


//This function is the manuel play algorithm. Each step takes an arrow input from user and checks the new positions. 
//After that it updates the user location on the map.
//If user can reach the exit, it saves the collected particules and calculates the score and updates user's score and save it to userdata.
void manuelGameplay(UserAccount *loginaccount){

    
    char gamemode;   // switch case expression
    int flag=0;  // do while condition for menu
    char *map;   // map name to load 
    char map2[30];
    do{
       system("cls");
       printf("\n***********MANUEL GAMEMODE**********");
       printf("\n\tChoose a map :");
       printf("\n\t1.MAP 1");
       printf("\n\t2.MAP 2");
       printf("\n\t3.MAP 3");
       printf("\n\t4.Load your own map");
       printf("\n\tYour Choice :");
       scanf("%c",&gamemode); 
       switch (gamemode)
       {
       case '1':
        map="map1.txt";
        flag=1;
        break;
       case '2':
        map="map2.txt";
        flag=1;
        break; 
       case '3':
        map="map3.txt";
        flag=1;
        break;
       case '4':
        printf("\nEnter the name.txt of map that you want to play:");
        scanf(" %s",map2);
        map=map2;
        FILE *f =fopen(map,"r");
        if(f==NULL){
            printf("\n There is no such a file %s !!",map);
            sleep(1);
        }
        else flag=1;
        
        break; 

       default:
        printf("\nInvalid Option!!");
        sleep(1);
        break;
       }
    }while(flag==0);
    system("cls");
    printf("\n\tLoading your game...");
    sleep(1);
    system("cls");
    char **labyrnth=readLabyrinth(map);
    //printLabyrinth(labyrnth);


    char player='X';
    int playerR=2; // Player's location on the map
    int playerC=1;
    int i,j,k;
	char a;
    int flag2=0; //flag for while loops
    char collection[20]={'0'};
    int number_of_collects=0; 
    int exit=0; // exit condition
    int gameover=0; // Gameover condition
    int time=1;
    clock_t start = clock(); //Starts the time
    clock_t end = clock();
	while ((end - start)/CLOCKS_PER_SEC<30&&exit!=1&&gameover!=1)
	{
		printf("Geri Sayim:%d\n",31-((end - start)/CLOCKS_PER_SEC));
        printLabyrinth(labyrnth);
        printf("\nCollected items : ");
        for(k=0;k<number_of_collects;k++){
            printf("%c ",collection[k]);
        }
		a = getch();
		if (a==27) 
		{
			printf("Cikis\n");
            exit=1;
			
		}
		else
		{   
            if(exit!=1){
            do{
            a = getch();
			//printf("Bastiginiz ok tusu:%d\n",a);
            
            switch (a)
            {
            case 'H': // UP UP UP Conditions
                
                if(playerR-1>=0){
                  if(labyrnth[playerR-1][playerC]!='1'){
                    if(labyrnth[playerR-1][playerC]=='p'){
                        collection[number_of_collects]='p';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR-1][playerC]=='P'){
                        collection[number_of_collects]='P';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR-1][playerC]=='e'){
                        collection[number_of_collects]='e';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR-1][playerC]=='E'){
                        collection[number_of_collects]='E';
                        number_of_collects++;
                    }
                     else if(labyrnth[playerR-1][playerC]=='C'){
                        exit=1;
                    }
                    else if(labyrnth[playerR-1][playerC]=='K'){
                        gameover=1;
                        
                    }
                    
                  labyrnth[playerR][playerC]='0';
                  playerR-=1;
                  labyrnth[playerR][playerC]=player;
                  }
                }
                flag2=1;
                
                break;
            case 'P': // DOWN DOWN Conditions
                
                if(playerR+1<=5){
                  if(labyrnth[playerR+1][playerC]!='1'){
                    if(labyrnth[playerR+1][playerC]=='p'){
                        collection[number_of_collects]='p';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR+1][playerC]=='P'){
                        collection[number_of_collects]='P';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR+1][playerC]=='e'){
                        collection[number_of_collects]='e';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR+1][playerC]=='E'){
                        collection[number_of_collects]='E';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR+1][playerC]=='C'){
                        exit=1;
                        
                    }
                    else if(labyrnth[playerR+1][playerC]=='K'){
                        gameover=1;
                        
                    }
                  labyrnth[playerR][playerC]='0';
                  playerR+=1;
                  labyrnth[playerR][playerC]=player;
                  }
                }
                flag2=1;
                
                break;    
            
            case 'M': // RIGHT RIGHT Conditions
                
                if(playerC+1<=9){
                  if(labyrnth[playerR][playerC+1]!='1'){
                    if(labyrnth[playerR][playerC+1]=='p'){
                        collection[number_of_collects]='p';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC+1]=='P'){
                        collection[number_of_collects]='P';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC+1]=='e'){
                        collection[number_of_collects]='e';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC+1]=='E'){
                        collection[number_of_collects]='E';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC+1]=='C'){
                        exit=1;
                    }
                    else if(labyrnth[playerR][playerC+1]=='K'){
                        gameover=1;
                        
                    }
                  labyrnth[playerR][playerC]='0';
                  playerC+=1;
                  labyrnth[playerR][playerC]=player;
                  }
                }
                flag2=1;
                
                break;


            case 'K': // LEFT LEFT Conditions
                
                if(playerC-1>=0){
                  if(labyrnth[playerR][playerC-1]!='1'){
                    if(labyrnth[playerR][playerC-1]=='p'){
                        collection[number_of_collects]='p';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC-1]=='P'){
                        collection[number_of_collects]='P';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC-1]=='e'){
                        collection[number_of_collects]='e';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC-1]=='E'){
                        collection[number_of_collects]='E';
                        number_of_collects++;
                    }
                    else if(labyrnth[playerR][playerC-1]=='C'){
                        exit=1;
                        
                    }
                    else if(labyrnth[playerR][playerC-1]=='K'){
                        gameover=1;
                        
                    }
                  labyrnth[playerR][playerC]='0';
                  playerC-=1;
                  labyrnth[playerR][playerC]=player;
                  }
                }
                flag2=1;
                
                break;    
            default:
                printf("\nInvalid Button");
                
                break;
            }

            }while(flag2==0);
            }
            
		}
		//sleep(1); 
        system("cls"); 
        end=clock();
	}


    if(exit==1){
        printf("EXITED!!\n");
        printf("collected items : ");
        for(k=0;k<number_of_collects;k++){
            printf("%c ",collection[k]);
        }
        //counts for elements
        int antimadde=0;
        int proton=0;
        int nproton=0;
        int elektron=0;
        int nelektron=0;
        for(k=0;k<number_of_collects;k++){
            if(collection[k]=='P')proton++;
            else if(collection[k]=='p')nproton++;
            else if(collection[k]=='E')elektron++;
            else nelektron++;
        }
        if(nproton>proton&&elektron>nelektron){
            if(nproton-proton<elektron-nelektron)antimadde=nproton-proton;
            else antimadde=elektron-nelektron;

        }
        //reading userdata from file
        UserAccount *head=readUserData();
        UserAccount *ptr=head;
        
        //calculating score and antimatters
        int index=1;
        while(ptr!=NULL&&strcmp(ptr->nickname,loginaccount->nickname)!=0){
            index++;
            ptr=ptr->next;
        }
        int newscore=ptr->score;
        newscore+=antimadde*5 + number_of_collects;
        ptr->score=newscore;

        printf("\nYou have produced %d antimatters  Your new score :%d ",antimadde,newscore);
        FILE *newdata = fopen("userdata4.bin","wb");    
        if(!newdata){
            perror("\nERROR!");
        }     
        while(head->next!=NULL){
            fwrite(head,sizeof(UserAccount),1,newdata); // saving updated scores data
            head=head->next;
        }
        fclose(newdata);    
    }
    else if(gameover==1){
	printf("BLACK HOLE !");   
    }
    else{
        printf("Your Time is UP!!");
    }

}


//This is the basic gamemenu we call the gameplay and login functions here.
void gamemenu(UserAccount *loginaccount,UserAccount *head){
    sleep(2);
    char a;
    int return_Option;
    int mode;
    int flag3=1; // flag for while loop
    UserAccount *headptr=head;
    
    //////////// AUTOPLAY ///////////
    char *collectables=calloc(20,sizeof(char)); //allocating array for collects
    char **labelMatrix=calloc(6,sizeof(char**)); // an matrix for labeling player's past position
    int k;
    for(k=0;k<6;k++){
        *(labelMatrix+k)=calloc(11,sizeof(char*));
    }
    int m,n,l;

    int *coordinates=(int*)calloc(2,sizeof(int));
                
    /////////////////////////////////
    


    do
    {
        
        system("cls");
        printf("\nWelcome %s\n",loginaccount->nickname);
        printf("***********MENU***********\n");
        printf("\t1.HOW TO PLAY\n");
        printf("\t2.SCORE TABLE\n");
        printf("\t3.START TO PLAY\n");
        printf("\n\t Your Choice : ");
        scanf(" %c",&a);
        switch (a)
        {
        case '1':
            system("cls");
            printf( "1. In the maze, the following particles are present:\n"
                 "   P+  : proton (P)\n"
                 "   e-  : electron (e)\n"
                 "   P-  : anti-proton (p)\n"
                 "   e+  : positron (E)\n"
                 "2. To produce anti-hydrogen at the exit of the maze, you must only have P- and e+ particles in your possession.\n"
                 "3. When a particle collides with its opposite-signed counterpart, they annihilate each other.\n"
                 "4. Black holes are denoted by K. If you pass through the cells where black holes are present, the game ends.\n"
                 "5. If the user fails to reach the exit within a certain game duration, the game ends.\n"
                 "6. Each particules are 1 point and antimatters are 5 point.");
            printf("\nTo return back press any button : ");
            return_Option= getch();
            
            
            break;
        
        case '2':
            system("cls");
            printf("\n*******SCORE TABLE*******\n");
            headptr=readUserData(); // Reading list from user data
            UserAccount *ptr=headptr;
            if(ptr->next!=NULL){
                 ptr=sortLinkedList(ptr);
            }
            
            int i=1;
            while(ptr->next!=NULL){
                printf("\t%d.%s Score : %d \n",i,ptr->nickname,ptr->score); //Printing Score Table
                ptr=ptr->next;
                i++;

            }
            printf("\nTo return back press any button : ");
            return_Option= getch();
              
            break;

        case '3': 
             do{
             system("cls");
             printf("\n**************Game Mode****************");
             printf("\n\t1.Manuel");
             printf("\n\t2.Auto");
             printf("\n\t3.Back to previous menu ");
             printf("\n\tYour Choice : ");
             scanf("%d",&mode);
             switch (mode)
             {
             case 1:
                manuelGameplay(loginaccount); // calling manuel gameplay for case 1;
                sleep(1);
                printf("\n Game is finished!");
                printf("\nGoing back to menu...");
                sleep(2);
                flag3=0;
                break;

             case 2:
                for(l=0;l<20;l++){
                    collectables[l]='\0';
                }
                for(m=0;m<6;m++){
                    for(n=0;n<10;n++){
                        labelMatrix[m][n]='0';
                    }
                }
                coordinates[0]=2;
                coordinates[1]=1;
                char **labyrnt=readLabyrinth("map1.txt");
                autoGameplay(labyrnt,2,1,labelMatrix,coordinates,collectables,0);//calling autogameplay for case 2
                int i=0;
                printf("\nCollected Particules : ");
                while(collectables[i]!='\0'){
                    printf("%c,",collectables[i]);
                    i++;
                }

                //Counts of particules
                int antimadde=0;
                int proton=0;
                int nproton=0;
                int elektron=0;
                int nelektron=0;
                for(k=0;k<i;k++){
                    if(collectables[k]=='P')proton++;
                    else if(collectables[k]=='p')nproton++;
                    else if(collectables[k]=='E')elektron++;
                    else nelektron++;
                }
                if(nproton>proton&&elektron>nelektron){
                  if(nproton-proton<elektron-nelektron)antimadde=nproton-proton;
                  else antimadde=elektron-nelektron;
                }
                printf("\n Produced %d Antimatters ",antimadde);
                

                sleep(4);
                break;
             case 3:
                flag3=0;
                break;       
             
             default:
                printf("\nInvalid Option Try Again !");
                flag3=1;
                sleep(1);
                break;
             }
             
             }while(flag3==1);
             


             break;   

        default:
            printf("\nInvalid Option Try Again !");
            sleep(1);
            break;
        }
    } while (1);
    

}

int main(){
    // we read data from file and create a linked list.
    UserAccount *head=readUserData();
   
    UserAccount *loginAccount;
    loginAccount=loginmenu(head);// logined account or signed up
    gamemenu(loginAccount,head);// Starts to play 
    

    
}