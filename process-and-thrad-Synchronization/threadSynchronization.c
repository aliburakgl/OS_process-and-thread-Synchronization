#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h> 
pthread_t tid[10];  
struct node 
{ 
    int key; 
    struct node *left, *right; 
}; 
   
struct node *newNode(int newKey) 
{ 
    struct node *temp =  (struct node *)malloc(sizeof(struct node)); 
    temp->key = newKey; 
    temp->left = NULL;
    temp->right = NULL; 
    return temp; 
} 
void inorder(struct node *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        printf("%d \n", root->key); 
        inorder(root->right); 
    } 
}
int cntIndex=0;
void reverseOrderedBST(struct node *root, int *arr)
{
	if (root != NULL) 
	{ 
		reverseOrderedBST(root->left,arr);
		arr[cntIndex++]=root->key; 
		reverseOrderedBST(root->right,arr); 
	} 
}   
struct node* insert(struct node* node, int key) 
{ 
    // Agacin bos olup olmama durumu kontrol ediliyor
    if (node == NULL) return newNode(key); 
    // Dugum ekleme
    if (key <= node->key) 
        node->left  = insert(node->left, key); 
    else if (key > node->key) 
        node->right = insert(node->right, key);    
    return node; 
} 
struct node * leftMostNode(struct node* node) 
{ 
    struct node* current = node; 
  
    while (current && current->left != NULL) 
        current = current->left; 
  
    return current; 
} 
  
struct node* deleteNode(struct node* root, int key) 
{ 
    // base case 
    if (root == NULL) return root; 
  
    if (key < root->key) 
        root->left = deleteNode(root->left, key); 
    else if (key > root->key) 
        root->right = deleteNode(root->right, key); 
    else
    { 
        if (root->left == NULL) 
        { 
            struct node *temp = root->right; 
            free(root); 
            return temp; 
        } 
        else if (root->right == NULL) 
        { 
            struct node *temp = root->left; 
            free(root); 
            return temp; 
        } 
  
        struct node* temp = leftMostNode(root->right); 
        root->key = temp->key; 
        root->right = deleteNode(root->right, temp->key); 
    } 
    return root; 
} 
struct node *root = NULL; 
typedef struct{
char * input;
int arr[1]; //arr[0]=k
}params;
int count;
pthread_mutex_t lock; 
void* operation(void * args)
{	
	params * pr= args;
	// Dosyadan okuma
	int num;
	FILE * fp;
	fp = fopen(pr->input, "r");
	while(fscanf(fp,"%d",&num) != EOF) // Girdi dosyalarinin okumasu
	{
		pthread_mutex_lock(&lock); // kilitleme
		if(count < pr->arr[0])
		{
			if(count == 0)
				root = insert(root,num);
			else
				insert(root,num);			
			count++;
		}
		else
		{
			if(leftMostNode(root)->key < num)
			{
				struct node *min = leftMostNode(root);
				root = deleteNode(root,min->key);
				insert(root,num);		
			}
		}
		pthread_mutex_unlock(&lock);  //kilit acma
	} 
	fclose(fp);
	free(pr);
	pthread_exit(NULL);	
}


int main(int argc, char *argv[]) 
{ 
   	struct timeval startTime, endTime;    	
	long elapsedTime;
	gettimeofday(&startTime, NULL);
	// k , N , # of N arg, output
	if(argc < 5 && argc > 14){
		printf("Wrong number of arguments \n");
		exit(1);
	}
	int k, n;

	k = atoi(argv[1]);
	n = atoi(argv[2]);
	if(k<100 || k>10000){
		printf("K is out of bounds \n");
		exit(1);	
	}
	if(n<1 || n>10){
		printf("N is out of bounds \n");
		exit(1);	
	}

	if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    	} 
	count =0;
	
	for(int i=0;i<n;i++){ // Tum threadleri operasyon fonksiyonuna gonderiyoruz
		char* infile = argv[(3+i)];
		params * pr = malloc(sizeof(*pr));	
		pr->input = infile;
		pr->arr[0]= k;
		// Thread olusturma
		int ret = pthread_create(&(tid[i]),NULL,operation,pr);
		if(ret != 0){
			printf("thread create failed\n");
			exit(1);
		}				
	}
	//Thread Join
	for(int l =0; l<n;l++)	
		pthread_join(tid[l],NULL);
	pthread_mutex_destroy(&lock);
	int * temp = (int *)malloc(k*sizeof(int));
	reverseOrderedBST(root,temp);
	FILE * fpw;
	fpw = fopen(argv[(3+n)], "w");	
	for(int j=k-1; j>=0;j--)
	{
		fprintf(fpw,"%d\n",temp[j]);
	}
	fclose(fpw);	
	free(temp);
	gettimeofday(&endTime, NULL);
	elapsedTime = (endTime.tv_sec - startTime.tv_sec) * 1000000 +
	endTime.tv_usec - startTime.tv_usec;
	printf("findtopk in ms: %ld\n",  elapsedTime);
    return 0; 
} 
