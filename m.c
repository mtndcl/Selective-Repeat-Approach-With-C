#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>


typedef struct dropindex {

    struct dropindex * next;
    
    int data;

} dropindex_t;

dropindex_t *filehead=NULL;
dropindex_t *filetail=NULL;
 
void Add(int data){
    

    if(filehead==NULL){

    	 dropindex_t *p = (dropindex_t *)malloc(sizeof(dropindex_t));
       	p->next = NULL;
      
      	
       	p->data=data;
       	
       	
       	filehead=p;
       	filetail=filehead;


    }else {
    	
    	dropindex_t *p = (dropindex_t *)malloc(sizeof(dropindex_t));
      p->next = NULL;
    	p->data=data;
    	filetail->next=p;
    	filetail=p;
    	

    }
     
}

void init(){
  

  printf("<SIMULATION TIME> (00:00:00)\n");
  printf("<EVENTS>\n");
  printf("- Data size sent so far is 0.00 Byte\n");
  printf("<WINDOW STATE>: empty\n");


}
void ReadInputFile(char *filename){


  FILE  *inputfile;
  if((inputfile = fopen(filename, "r")) == NULL) {
         printf("No such %s file\n", filename);
           exit(1);
    }
    int d;
    while(fscanf(inputfile, " %d", &d) != EOF){  

      Add(d);

    }

}


typedef struct node {
  
    
    struct node * next;
    struct node * prev;

    int data;

    int timeelapsed;
  
    int  roundtriptime;
    int  timeout;

    int droptime;
    
    int achived;
    int candrop;

    int packetsize;

    int go;
    int passedtime;


} node_t;
node_t  *head=NULL;
node_t  *tail;
int currentsize=0;
float totalsendenddata=0;
int dropnumber=0;
int sumtime=0;
int DropTime(int data){
  

  dropindex_t *temp=filehead;

  int x=0;
  for(temp;temp!=NULL;temp=temp->next){
      

      //printf("data :  %d \n",temp->data);
      if(data==temp->data){
          dropnumber++;
          x++;
      }
  }
  return x;
    
}
void Dequeue(){


  node_t *temp=head;
  
  if(head->next!=NULL){


    //printf("gone %d \n",head->data);
    head->next->prev=NULL;
    head=head->next;


    
  }else {
    
    head=NULL;
  }
  


}


void Enqueue(int data,int Timeout,int Round_Trip_Time,int packetsize){
    
    
    printf("- A new data packet (id:%d) has now been sent!\n",data);
    if(head==NULL){

      node_t *p = (node_t *)malloc(sizeof(node_t));
        p->next = NULL;
      
        p->timeout=Timeout;
        p->roundtriptime=Round_Trip_Time;
        p->prev=NULL;
        p->achived=0;
        p->go=1;
        p->data=data;
        p->droptime=DropTime(data);
        p->timeelapsed=1;
        p->candrop=0;
        p->packetsize=packetsize;
        p->passedtime=0;
        head=p;
        tail=head;


    }else {
      
      node_t *p = (node_t *)malloc(sizeof(node_t));
      
      p->next = NULL;
      p->achived=0;
      p->data=data;
      p->packetsize=packetsize;
      p->droptime=DropTime(data);
      p->timeelapsed=1;
      p->timeout=Timeout;
      p->candrop=0;
       p->go=1;
        p->passedtime=0;
      p->roundtriptime=Round_Trip_Time;
      p->prev=tail;
      tail->next=p;
      tail=p;
    }
     
}
void SetTime(int mytime){
  

  int hours = mytime / 3600;
  int minutes = (mytime % 3600) / 60;
  int seconds = mytime % 60;
  printf("<SIMULATION TIME> (%02d:%02d:%02d)\n",hours,minutes,seconds);
  printf("<EVENTS>\n");
  

}
void printWindowState(){
  

   printf("- Data size sent so far is %0.02f Byte\n",totalsendenddata);
   printf("<WINDOW STATE> ");
  if(head==NULL){
    
    printf("empty\n");
  }else{
    
    node_t  *temp=tail;
    for(temp;temp!=NULL;temp=temp->prev){
    
      printf("%d -> %d",temp->data,temp->timeelapsed);
      if(temp->prev!=NULL){
        printf(" | ");
      }
    }
    printf("\n");
  }


}


void  Fix(){
  
    node_t *temp;
    if(head!=NULL){
      
        if(head->next!=NULL){
        temp=head->next;
        for(temp;temp!=NULL;temp=temp->next){
            
          
            if(temp->go==0){
              
                temp->timeelapsed=temp->roundtriptime;
                temp->go=1;
            }

            if(temp->timeout+1==temp->timeelapsed){
              temp->droptime--;
              temp->timeelapsed=1;
              
              printf("- It is timeout for data packet (id:%d), so it has been resent now!0\n",temp->data);
            }
          
             
        }  
      }
    }



}
void DeleteElement(){
  
    node_t  *temp;
    for(temp=head;temp!=NULL;temp=temp->next){
      
        temp->passedtime++;
    }
    if(head->next!=NULL){
      temp=head->next;
      for(temp;temp!=NULL;temp=temp->next){
          
        if(temp->go){
          temp->timeelapsed++;
        }
        if(temp->timeelapsed>temp->roundtriptime){
          temp->go=0;
          
        }
        if(temp->droptime>0){
            temp->go=1;
        }
           
      }  
    }
    //printf("data : %d elapd : %d round : %d\n",head->data,head->timeelapsed,head->roundtriptime);
    head->timeelapsed++;
    if(head->droptime>0){

        if(head->timeelapsed==head->timeout+1){
        

          printf("- It is timeout for data packet (id:%d), so it has been resent now!0\n",head->data);
          head->droptime--;
          head->timeelapsed=1;

        }
        

        //////Check achived
        if(head->next!=NULL){
          temp=head->next;
          for(temp;temp!=NULL;temp=temp->next){
              if(temp->timeelapsed==temp->roundtriptime+1  &&  temp->droptime==0 && temp->achived==0 ){
                  printf("- Data packet (id:%d) has now been ACK'ed by receiver!1\n",temp->data);
                  temp->achived=1;
                  
                  totalsendenddata=totalsendenddata+temp->packetsize;

                  sumtime+=temp->passedtime;
              }
          } 
        }
      
        

    }else if(head->timeelapsed==head->roundtriptime+1){
        



          


          
         
          if(head->achived==0){
            printf("- Data packet (id:%d) has now been ACK'ed by receiver!0\n",head->data);
            totalsendenddata=totalsendenddata+head->packetsize;
            sumtime+=head->passedtime;
          }

          temp=head;
          for(temp;temp!=NULL;temp=temp->next){
              

              ///printf("(data id:%d) elapsed time%d 0\n",temp->data,temp->timeelapsed);
              if(temp->timeelapsed==temp->roundtriptime+1){
                  
                  
                  temp->candrop=1;
              }else{

                break;
              }
             
          }
          
         
          
          temp=head;
          for(temp;temp!=NULL;temp=temp->next){
            
              if(temp->candrop){
                
                  currentsize--;
                 //printf("- xx delete %d \n",temp->data);
                  Dequeue();
                 //printf("- D\n");
              }
          }
         
          
          
          

    }
    
  
    
    
    
    


}
void main(int argc, char *argv[]) {
	

	int windowsize=atoi(argv[1]);
	
	int datasize=atof(argv[2]);
	
	int packetsize=atof(argv[3]);
	
	int timeout=atoi(argv[4]);
	
	int roundtriptime=atoi(argv[5]);
	
  int sendeddata=0;

  

	ReadInputFile(argv[6]);

    
  
  int data=0;

  int currenttime=1;
  int remaindata=datasize+packetsize;
  int numberbiit=datasize/packetsize;
  int r=datasize%packetsize;
  if(r>0){
    numberbiit++;
  }
  while(numberbiit!=data){
      

      
      //printf("sended data  %d \n",sendeddata);
      
      
      printf("\n");
      SetTime(currenttime);
     

      if(head!=NULL){
        DeleteElement();
      }
      if(currentsize<windowsize){
        
          sendeddata=sendeddata+packetsize;
          remaindata=remaindata-packetsize;
          if(remaindata<packetsize){
            packetsize=remaindata;
          }
          
          
          Enqueue(data++,timeout,roundtriptime,packetsize);
          
          currentsize++;
      }
      Fix();
      printWindowState();

      //DeleteElement(packetsize,roundtriptime);
      currenttime++;
     
      
  }

  while(head!=NULL){
    
    printf("\n");
    SetTime(currenttime);
    
      DeleteElement();
      
      Fix();
     
      printWindowState();
     
      currenttime++;
   
    
  }


  
  int p=atof(argv[3]);
  
  float ave=(float)sumtime/(float)data;
  printf("\n");
  printf("***************************************************\n");
  printf("*                 TRANSFER REPORT                 *\n");
  printf("Parameter Setting:                                 \n");
  printf("---------------------------------------------------\n");
  printf("Window Size                          :        %02d\n",windowsize);
  printf("Timeout                              :        %04d Sec.\n",timeout);
  printf("RTT                                  :        %04d Sec.\n",roundtriptime);
  printf("Data Size                            :        %04d Byte\n",datasize);
  printf("Packet Size                          :        %04d Byte\n",p);
  printf("---------------------------------------------------\n");
  printf("Results:                                           \n");
  printf("---------------------------------------------------\n");
  printf("Number of packet to send the data    :        %04d\n",data);
  printf("Number of packet dropped             :        %04d\n",dropnumber);
  printf("Average time to send a single packet :        %.3f Sec.\n",ave);
  printf("***************************************************\n");


			
}