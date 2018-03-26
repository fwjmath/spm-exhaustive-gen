#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 300

int n, endptr=0, spm[MAXLEN], pool[MAXLEN], poolend=0;
long long enumcount=0;
FILE* fio;

__inline void snapshot(){
	/*int i=0;
	while(spm[i]!=0){
		printf("%d ",spm[i]);
		i++;
	}
	printf("\n");*/
	enumcount++; 
	return;
}

__inline void initcomb(int* ptrs, int cnt, int startp, int endp){
	int i;
	if(cnt==0) return;
	if(cnt==endp-startp+1){
		for(i=startp;i<=endp;i++) spm[i]++;
		return;
	}
	if(((endp-startp+1)>>1)>=cnt){
		for(i=0;i<cnt;i++){
			ptrs[i]=startp+cnt-i-1;
			spm[ptrs[i]]++;
		}
	}else{
		int mycnt=endp-startp+1-cnt;
		for(i=startp;i<=endp;i++){
			spm[i]++;
		}
		for(i=0;i<mycnt;i++){
			ptrs[i]=startp+mycnt-i-1;
			spm[ptrs[i]]--;
		}
	}
	return;
}

// contract: cnt <= endp-startp+1
__inline int nextcomb(int* ptrs, int cnt, int startp, int endp){
	if((cnt==0)||(cnt==endp-startp+1)) return -1;
	if(((endp-startp+1)>>1)>=cnt){
		int top=endp, i=0;
		while((ptrs[i]==top)&&(i<cnt)){
			i++;
			top--;
		}
		if(i==cnt) return -1;
		spm[ptrs[i]]--;
		ptrs[i]++;
		spm[ptrs[i]]++;
		i--;
		while(i>=0){
			spm[ptrs[i]]--;
			ptrs[i]=ptrs[i+1]+1;
			spm[ptrs[i]]++;
			i--;
		}
	}else{
		int mycnt=endp-startp+1-cnt;
		int top=endp, i=0;
		while((ptrs[i]==top)&&(i<mycnt)){
			i++;
			top--;
		}
		if(i==mycnt) return -1;
		spm[ptrs[i]]++;
		ptrs[i]++;
		spm[ptrs[i]]--;
		i--;
		while(i>=0){
			spm[ptrs[i]]++;
			ptrs[i]=ptrs[i+1]+1;
			spm[ptrs[i]]--;
			i--;
		}
	}
	return 0;
}

__inline void destroycomb(int* ptrs, int cnt, int startp, int endp){
	if(cnt==0) return;
	if(cnt==endp-startp+1){
		for(int i=startp;i<=endp;i++) spm[i]--;
		return;
	}
	if(((endp-startp+1)>>1)>=cnt){
		for(int i=0;i<cnt;i++) spm[ptrs[i]]--;
	}else{
		int mycnt=endp-startp+1-cnt;
		for(int i=startp;i<=endp;i++) spm[i]--;
		for(int i=0;i<mycnt;i++) spm[ptrs[i]]++;
	}
	return;
}

// return the count of a reduced sand pile with *grain* grains of sand of width *width*, with at least one 0
__inline void getpilenum(int grain, int width){
	if(grain==0){
		snapshot();
		return;
	}
	if(width==1) return;
	if(grain<0) return;
	int leading, onecount, upmin;
	// when leading 0 is on top
	if(grain<=width-1){
		int* localcomb=&pool[poolend];
		poolend+=grain;
		initcomb(localcomb,grain,1,width-1);
		snapshot();
		while(nextcomb(localcomb,grain,1,width-1)==0) snapshot();
		poolend-=grain;
		destroycomb(localcomb,grain,1,width-1);
	}
	// when leading 0 is on 2nd row
	for(int i=0;i<=grain-1;i++){
		if(i>width-2) break;
		int* localcomb=&pool[poolend];
		poolend+=i;
		spm[0]+=grain-i;
		initcomb(localcomb,i,2,width-1);
		snapshot();
		while(nextcomb(localcomb,i,2,width-1)==0) snapshot();
		poolend-=i;
		spm[0]-=grain-i;
		destroycomb(localcomb,i,2,width-1);
	}
	if(width==2){
		return;
	}
	// enumerate the position of leading 0
	for(leading=3;leading<=width;leading++){
		// enumerate the number of 1 after the leading 0
		for(onecount=0;onecount<=width-leading;onecount++){
			int* localcomb=&pool[poolend];
			poolend+=onecount;
			initcomb(localcomb,onecount,leading,width-1);
			do{
				// upper part has *grain-onecount* grains of sand
				// enumerate possibilities of minimum of upper part
				upmin=1;
				for(int i=0;i<leading-1;i++) spm[i]++;
				while(grain-upmin*(leading-1)-onecount>=0){
					getpilenum(grain-upmin*(leading-1)-onecount,leading-1);
					upmin++;
					for(int i=0;i<leading-1;i++) spm[i]++;
				}
				for(int i=0;i<leading-1;i++) spm[i]-=upmin;
			}while(nextcomb(localcomb,onecount,leading,width-1)==0);
			poolend-=onecount;
			destroycomb(localcomb,onecount,leading,width-1);
		}
	}
	return;
}

__inline void enumerate(){
	int i=1, j, k;
	while(i*(i+1)<=n*2){
		for(j=0;j<i;j++){
			spm[j]=i-j;
		}
		getpilenum(n-i*(i+1)/2,i+1);
		printf("Finished width %d\n",i);
		i++;
	}
	return;
}

int main(){
	fio=fopen("output_cnt.txt","a");
	printf("Counting Sand Pile Models\nPlease enter the number of sand grains: ");
	scanf("%d",&n);
	printf("\nCounting...\n");
	enumerate();
	printf("Finished counting. Result: %lld",enumcount);
	fprintf(fio,"%d, %lld\n",n,enumcount);
	fclose(fio);
	// Timing
	
	char c;
	scanf("%c",&c);
	scanf("%c",&c);
	
	// End of timing
	return 0;
}
