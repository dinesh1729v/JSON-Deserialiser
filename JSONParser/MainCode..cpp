#include<stdlib.h>
#include<stdio.h>
struct phone
{
	char type[100],number[20];
	struct phone *next;
};
struct basics
{
	char firstname[100],lastname[100];
	char age[10];
	char height[10];
	struct phone *phoneNumber;
	struct basics *next;
};

bool stringCmp(char *string1, char *string2)
{
	while (*string1 != '\0')
	{
		if (*string1 != *string2) return 0;
		string1++;
		string2++;
	}
	if (*string2 != '\0') return 0;
	return 1;
}
bool isValid(FILE *fp)
{
	char ch;
	int f[5] = { 0, 0, 0 };
	bool flag = 0, colflag = 0, comflag = 0;
	while (!feof(fp))
	{
		int i;
		ch = fgetc(fp);
		if (ch == '{')f[0]++;
		else if (ch == '}')f[0]--;
		else if (ch == '[')f[2]++;
		else if (ch == ']')f[2]--;
		else if (ch == '"'){
			if (!flag)f[1]++;
			else f[1]--;
			flag = !flag;
		}
		else if (ch == ':')
			colflag = !colflag;
		else if (ch == ',')
			comflag = !comflag;
	}
	fclose(fp);
	return (flag == 0 && colflag == 1 && comflag == 1 && !f[0] && !f[1] && !f[2]);//printf("Valid\n");
}
//given firstname and last name find all the phone numbers
void search(char *fname, char *lname, FILE *fp)
{
	
}
bool isAlpha(char ch){
	return ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122));
}
bool isNum(char ch){
	return (ch >= 48 && ch <= 57);
}
void fillData(struct basics *head, FILE *fp)
{
	char firstName[] = "firstName", lastName[] = "lastName", phoneNumbers[] = "phoneNumbers";
	char age[] = "age", height[] = "height", ch, key[100], value[100], type[] = "type", number[] = "number";
	bool keyOrValue = 0,pkeyOrValue=0;
	int cnt=0;
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (cnt == 5){
			cnt = 0;
			while ((ch = fgetc(fp)) != '{' && !feof(fp));
			if (feof(fp))break;
			else{
				head->next = (struct basics*)malloc(sizeof(struct basics));
				head->next->next = NULL;
				head = head->next;
			}
		}
		else if (ch == '"')
		{
			ch = fgetc(fp);
			if (!isAlpha(ch) && !isNum(ch))continue;
			int i = 0;
			while (ch != '"'){
				key[i++] = ch;
				ch = fgetc(fp);	
			}
			key[i] = '\0';
			if (stringCmp(key, firstName)){
				key[0] = '\0';
				ch = fgetc(fp);
				while ((ch = fgetc(fp)) != '"');
				cnt++;
				i = 0;
				ch = fgetc(fp);
				while (ch != '"'){
					head->firstname[i++] = ch;
					ch = fgetc(fp);
				}
				head->firstname[i] = '\0';
			}
			else if (stringCmp(key, lastName)){
				key[0] = '\0';
				ch = fgetc(fp);
				while ((ch = fgetc(fp)) != '"');
				cnt++;
				i = 0;
				ch = fgetc(fp);
				while (ch != '"'){
					head->lastname[i++] = ch;
					ch = fgetc(fp);
				}
				head->lastname[i] = '\0';
			}
			else if (stringCmp(key, age)){
				cnt++;
				key[0] = '\0';
				i = 0;
				while (!isNum(ch = fgetc(fp)));
				while (isNum(ch)){
					head->age[i++] = ch;
					ch = fgetc(fp);
				}
				head->age[i] = '\0';
			}
			else if (stringCmp(key, height)){
				cnt++;
				key[0] = '\0';
				i = 0;
				while (!isNum(ch = fgetc(fp)));
				while (isNum(ch)||ch=='.'){
					head->height[i++] = ch;
					ch = fgetc(fp);
				}
				head->height[i] = '\0';
			}
			else if (stringCmp(key, phoneNumbers)){
				cnt++;
				head->phoneNumber = (struct phone*)malloc(sizeof(struct phone));
				struct phone *p = head->phoneNumber;
				p->next = NULL;
				int pcnt = 0,i;
				char pkey[100];
				while ((ch = fgetc(fp)) != ']'){
					if (pcnt == 2){
						pcnt = 0;
						ch = fgetc(fp);
						while (ch != '{'&& ch!=']')
							ch = fgetc(fp);
						if (ch == ']')break;
						else {
							p->next = (struct phone*)malloc(sizeof(phone));
							p = p->next;
							p->next = NULL;
						}
					}
					else if(ch=='"'){
						ch = fgetc(fp);
						i = 0;
						if (!isAlpha(ch) && !isNum(ch))continue;
						while (ch != '"'){
							pkey[i] = ch;
							ch = fgetc(fp);
							i++;
						}
						pkey[i] = '\0';
						i = 0;
						if (stringCmp(pkey, type)){
							pkey[0] = '\0';
							pcnt++;
							ch = fgetc(fp);
							while ((ch = fgetc(fp) != '"'));
							ch = fgetc(fp);
							while (ch != '"'){
								p->type[i] = ch;
								ch = fgetc(fp);
								i++;
							}
							p->type[i] = '\0';
						}
						i = 0;
						if (stringCmp(pkey, number)){
							pcnt++;
							pkey[0] = '\0';
							ch = fgetc(fp);
							while ((ch = fgetc(fp) != '"'));
							ch = fgetc(fp);
							while (ch != '"'){
								p->number[i] = ch;
								ch = fgetc(fp);
								i++;
							}
							p->number[i] = '\0';
						}
					}
				}
			}
		}
	}
}
void search1(char *fname, char *lname, struct basics *head){
	int flag = 0;
	while (head != NULL){
		if (stringCmp(head->firstname, fname) && stringCmp(head->lastname, lname)){
			flag++;
			struct phone *temp = head->phoneNumber;
			while (temp != NULL){
				printf("type:%s\nnumber:%s\n", temp->type, temp->number);
				temp = temp->next;
			}
		}
		head = head->next;
	}
	if (!flag)printf("No such record found...\n");
}
void search2(char *fname, struct basics *head){
	int flag = 0;
	while (head != NULL){
		if (stringCmp(fname, head->firstname)){
			flag++;
			struct phone *temp = head->phoneNumber;
			while (temp != NULL){
				printf("type:%s\nnumber:%s\n", temp->type, temp->number);
				temp = temp->next;
			}
		}
		head = head->next;
	}
	if (!flag)printf("No such record/records found...\n");
}
void search3(char *age, struct basics *head){
	int flag = 0;
	while (head != NULL){
		if (stringCmp(age, head->age)){
			flag++;
			struct phone *temp = head->phoneNumber;
			while (temp != NULL){
				printf("type:%s\nnumber:%s\n", temp->type, temp->number);
				temp = temp->next;
			}
		}
		head = head->next;
	}
}
int main()
{
	FILE *fp = fopen("input.txt", "r");
	char fname[100], lname[100],age[4];
	if (!isValid(fp)){
		printf("File contents not valid... Please correct the file\n");
		getchar();
		return 0;
	}
	fp = fopen("input.txt", "r");
	struct basics *head = (struct basics*)malloc(sizeof(struct basics)),*temp;
	temp = head;
	temp->next = NULL;
	fillData(temp, fp);
	temp = head;
	printf("1.Given firstname and lastname print all phone numbers...\n");
	printf("2.Given only firstname print all phone numbers of all persons...\n");
	printf("3.Given age of a person print all phone nunbers of all persons...\n");
	printf("Enter your choice...\n");
	int ch;
	scanf("%d", &ch);
	if (ch == 1){
		printf("Enter firstname and lastname:");
		scanf("%s %s", fname, lname);
		search1(fname, lname, temp);
	}
	else if (ch == 2){
		printf("Enter firstname:");
		scanf("%s", fname);
		search2(fname, temp);
	}
	else if (ch == 3){
		printf("Enter the age:");
		scanf("%s", age);
		search3(age, temp);
	}
	else printf("Invalid choice...\n");
	getchar();
	getchar();
	return 0;
}


//For printing...
/*
while (head != NULL)
	{
		printf("\n%s \n%s \n%s \n%s", head->firstname, head->lastname, head->age, head->height);
		struct phone *temp = head->phoneNumber;
		while (temp != NULL){
			printf("\n%s \n%s", temp->type, temp->number);
			temp = temp->next;
		}
		head = head->next;
	}
*/
//try1
/*
char keys[7][20] = { "irstName", "astName", "ge", "eight", "ype", "umber" };
	int f[5] = {0,0,0,0,0};
	char ch;
	bool flag = 0,colflag=0,comflag=0;
	while (!feof(fp))
	{
		int i;
		ch = fgetc(fp);
		if (ch == '{')f[0]++;
		else if (ch == '}')f[0]--;
		else if (ch == '[')f[2]++;
		else if (ch == ']')f[2]--;
		else if (ch == '"'){
			if (!flag)f[1]++;
			else f[1]--;
			flag = !flag;
		}
		else if (ch == ':')
		{
			if (!colflag)f[3]++;
			else f[3]--;
			colflag = !colflag;
		}
		else if (ch == ','){
			if (!comflag)f[4]++;
			else f[4]--;
			comflag = !comflag;
		}
		if (ch == ' ')
		{
			while (ch != ' ')
				ch = fgetc(fp);
			fseek(fp, -1, SEEK_CUR);
		}
		else if (ch == 'f')
		{
			i = 0;
			fseek(fp, 9, SEEK_CUR);
			cmpFirstName(fp, fname);
		}

	}
	if (flag == 0 && f[0] == 0 && f[1] == 0 && f[2] == 0 && f[3]==1)printf("Valid\n");

*/







//try 2

/*
if (cnt == 4)
		{ 
			cnt = 0;
			head->next = (struct basics*)malloc(sizeof(struct basics));
			head = head->next;
			head->next = NULL;
		}
		ch = fgetc(fp);
		if (ch == '"' && keyOrValue == 0)
		{
			 int i=0;
			while ((ch = fgetc(fp)) != '"'){
				key[i] = ch;
				i++;
			}
			key[i] = '\0';
			keyOrValue = !keyOrValue;	
		}
		if (stringCmp(key, firstName)){
			keyOrValue = !keyOrValue;
			while ((ch = fgetc(fp)) != '"');
			key[0] = '\0';
			int i = 0;
			while ((ch = fgetc(fp)) != '"')
				head->firstname[i++]=ch;
			head->firstname[i] = '\0';
			cnt++;
		}
		else if (stringCmp(key, lastName)){
			keyOrValue = !keyOrValue;
			while ((ch = fgetc(fp)) != '"');
			key[0] = '\0';
			int i = 0;
			while ((ch = fgetc(fp)) != '"')
				head->lastname[i++] = ch;
			head->lastname[i] = '\0';
			cnt++;
		}
		else if (stringCmp(key, age)){
			keyOrValue = !keyOrValue;
			key[0] = '\0';
			int i = 0;
			//ch = fgetc(fp);
			//
			while ((ch = fgetc(fp)) != ':');
			while((ch = fgetc(fp))==' ');
			while (ch>='0' && ch<='9'){
				head->age[i++] = ch;
				ch = fgetc(fp);
			}
			head->age[i] = '\0';
			cnt++;
		}
		else if (stringCmp(key, height)){
			keyOrValue = !keyOrValue;
			key[0] = '\0';
			int i = 0;
			//while ((ch = fgetc(fp)) != '"');
			ch = fgetc(fp);
			while (((ch) != ',')){
				ch = fgetc(fp);
				if (ch == '\n')break;
				head->height[i++] = ch;
			}
			head->height[i] = '\0';
			cnt++;
		}
		/*else if (stringCmp(key, phoneNumbers)){
			cnt++;
			keyOrValue = !keyOrValue;
			head->phoneNumber = (struct phone *)malloc(sizeof(phone));
			struct phone *temp=head->phoneNumber;
			key[0] = '\0';
			while (ch != ']')
			{
				while ((ch = fgetc(fp)) != '"');
				int i = 0;
				while ((ch = fgetc(fp)) != '"'){
					key[i] = ch;
					i++;
				}
				key[i] = '\0';
				if (stringCmp(key, type)){
					key[0] = '\0';
					while ((ch = fgetc(fp)) != '"');
					int i = 0;
					while ((ch = fgetc(fp)) != '"'){
						temp->type[i] = ch;
						i++;
					}
					temp->type[i] = '\0';
				}
				while ((ch = fgetc(fp)) != '"');
				i = 0;
				while ((ch = fgetc(fp)) != '"'){
					key[i] = ch;
					i++;
				}
				key[i] = '\0';
				if (stringCmp(key, number))
				{
					key[0] = '\0';
					while ((ch = fgetc(fp)) != '"');
					int i = 0;
					while ((ch = fgetc(fp)) != '"'){
						temp->number[i] = ch;
						i++;
					}
					temp->number[i] = '\0';
				}
				temp->next = (struct phone*)malloc(sizeof(struct phone));
				temp = temp->next;
				while ((ch = fgetc(fp)) != '}');
			}
			temp->next = NULL;
		}*/






/*struct offsets{
	int fname,lname,age,height,phno;
};

int IsExists(char  *fp)
{
	FILE *file;
	if (file = fopen(fp, "r"))
	{
		fclose(file);
		return 1;
	}
	return 0;
}
void createNewFile(char *fp)
{
	FILE *file;
	file = fopen(fp, "w");
	fclose(file);
}
void writeJSONToFile()
{

}
int main()
{
	FILE *f;
	if (!IsExists("offsets.bin"))
	{
		createNewFile("offsets.bin");
		f = fopen("offsets.bin", "rb+");
		writeJSONToFile(f,)
	}
	else
	{

	}
	return 0;

}
*/

/*int cmpFirstName(FILE *fp, char *fname)
{
	char ch;
	if (ch == ' ')
	{
		while (ch != ' ')
			ch = fgetc(fp);
		fseek(fp, -1, SEEK_CUR);
	}
}*/