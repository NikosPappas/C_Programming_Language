#include <stdio.h>
#include <string.h>
void dec_to_bin(int ,char *);
void reverse(char *);
void disp_bin(char *);
int main(void){
        int number;
	char str[80];
	printf("Value? (<0 to stop)");
	scanf("%d",&number);

	while(number>=0){
		dec_to_bin(number,str);
		reverse(str);
		disp_bin(str);
		puts("\n");
		printf("value ?");
		scanf("%d",&number);
	}
	return 0;
}
void disp_bin(char *str){
	int i;
	i=0;
	while(str[i++]!='\0'){
		printf("%c",str[i]);
		if(i%4==3){
			printf(" ");
		}

	}
}
void reverse(char *str){
	int temp,up,down;
	int len;
	len=strlen(str);
	for(up=0,down=len-1;up<down;up++,down--){
		temp=str[up];
		str[up]=str[down];
		str[down]=temp;
	}
}
void dec_to_bin(int val,char *str){
	int rem,count,type_size;
        type_size=sizeof(int)*8;
	for(count=0;count<type_size;count++){
		rem=val%2;
		if(rem){
			str[count]='1';
		}
		else{
			str[count]='0';
		}
		val=val/2;
	}
	str[count]='\0';
}
