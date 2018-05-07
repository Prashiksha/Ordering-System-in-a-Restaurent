// *******HEADER FILES USED*******

#include<iostream.h>
#include<conio.h>
#include<fstream.h>
#include<stdlib.h>
#include<stdio.h>


fstream fp;

// *******CLASS USED IN PROJECT*******

class product
{
  private:
	  int pno;
	  char name[50];
	  float price,discount;

  public:
	 void create_product()
	 {
	   cout<<"\nEnter the Product Number:";
	   cin>>pno;
	   cout<<"\nEnter the Product Name:";
	   gets(name);
	   cout<<"\nEnter the Product Price:";
	   cin>>price;
	   cout<<"\nEnter the Discount(%):";
	   cin>>discount;
	 }

	 void show_product()
	 {
	   cout<<"\n\nProduct Number:"<<pno;
	   cout<<"\n\nProduct Name:";
	   puts(name);
	   cout<<"\nProduct Price:"<<price;
	   cout<<"\n\nDiscount:"<<discount;
	 }

	 void alter()
	 {
	   cout<<"\n\nEnter the Product Name:";
	   gets(name);
	   cout<<"\nEnter the Product Price:";
	   cin>>price;
	   cout<<"\nEnter the Discount(%):";
	   cin>>discount;
	 }

	 int retpno()
	 {
	   return pno;
	 }

	 char* retname()
	 {
	   return name;
	 }

	 float retprice()
	 {
	   return price;
	 }

	 float retdis()
	 {
	   return discount;
	 }

}pr;



//*****function to write in file*****

void write_product()
{
  fp.open("Shop.dat",ios::out|ios::app|ios::binary);
  char a='y';
  do
  {
    pr.create_product();
    fp.write((char*)&pr,sizeof(product));
    cout<<"\n\nProduct Created";
    cout<<"\n\nDo you want to enter more products?(y/n):";
    cin>>a;
    cout<<"\n\n";
  } while(a=='y');
  fp.close();
  cout<<"\n\nPress ANY KEY to continue.";
}

//****search a specific record****

void display_sp(int n)
{
  int flag=0;
  fp.open("Shop.dat",ios::in|ios::binary);
  while(fp.read((char*)&pr,sizeof(product)))
  {
    if(pr.retpno()==n)
    {
      clrscr();
      cout<<"\n\nDETAILS OF THE SEARCHED PRODUCT:\n\n";
      pr.show_product();
      flag=1;
    }
  }
  fp.close();
  if(flag==0)
  {
    clrscr();
    cout<<"\nRecord Does Not Exist";
  }
  cout<<"\n\nPress ANY KEY to continue.\n";
}

//****Modification****

void modify_product()
{
  int num, found=0;
  cout<<"\n\nEnter the Product Number to be Modified :";
  cin>>num;
  fp.open("Shop.dat",ios::in|ios::out|ios::binary);

  while(fp.read((char*)&pr,sizeof(product))&&found==0)
  {
  if(pr.retpno()==num)
  {
    pr.show_product();
    found=1;
    int pos=-1*sizeof(pr);
    fp.seekp(pos,ios::cur);
    pr.alter();
    fp.write((char*)&pr,sizeof(product));
  }
}
fp.close();
if(found==0)
  cout<<"\nRecord Not Found";
cout<<"\n\nPress ANY KEY to continue.";
}

//*****Deletion***

void delete_product()
{
  int no,f=0,w=1;
  char ans='n';
  cout<<"\n\nEnter the Product Number to be Deleted.\n";
  cin>>no;
  fp.open("Shop.dat",ios::in|ios::out|ios::binary);
  fstream fp2;
  while(fp.read((char*)&pr,sizeof(product)))
  {
    if(pr.retpno()!=no)
    {
      cout<<"\n\nEntered product does not exist!Try Again\n";
      w=0;
    }
    break;
  }
  if(w==1)
  {

    fp2.open("Temp.dat",ios::out|ios::binary);
    fp.seekg(0);
    while(fp.read((char*)&pr,sizeof(product)))
    {
      if(pr.retpno()!=no)
      {
	fp2.write((char*)&pr,sizeof(product));
      }
      else
      {
	pr.show_product();
	cout<<"\n\nAre you sure you want to delete this data?(y/n)\n";
	cin>>ans;
	f=1;
	if(ans=='n')
	  fp2.write((char*)&pr,sizeof(product));
	else
	  ans='y';
      }
    }
    fp2.close();
    fp.close();
    remove("Shop.dat");
    rename("Temp.dat","Shop.dat");
    if(ans=='y'&&f==1)
      cout<<"\nRecord Deleted";
    else if(ans=='n'&&f==1)
      cout<<"\nDeletion cancelled";
    else
      cout<<"\nRecord not found.";

  }
  cout<<"\n\nPress ANY KEY to continue.";

}

//*****function to display all products price list*****

void menu()
{
  fp.open("Shop.dat",ios::in|ios::binary);
  if(!fp)
  {
    cout<<"Error!!! File could not be open\n\nGoto Admin Menu to Create File!";
  }
  else
  {
    cout<<"\n\n\t\t\tLIST OF PRODUCTS\n\n";
    cout<<"========================================================================\n";
    cout<<"Product No.\t\tProduct Name\t\tPrice\t\tDiscount";
    cout<<"\n========================================================================\n";
    fp.seekg(0);
    while(fp.read((char*)&pr,sizeof(product)))
    {
      cout<<pr.retpno()<<"\t\t\t"<<pr.retname()<<"\t\t\t"<<pr.retprice()<<"\t\t"<<pr.retdis()<<endl;
    }
    fp.close();
  }
}


//*********To place order & Generate Bill for Products******

void place_order()
{
  int order[50], qty[50], count=0,w;
  float amt, dis_amt, total=0;
  char ch='n';
  menu();
  cout<<"\n           PLACE YOUR ORDER\n";
  do
  {
    w=1;
    cout<<"\nEnter the Product Number:";
    cin>>order[count];
    fp.open("Shop.dat",ios::in||ios::binary);
    while(fp.read((char*)&pr,sizeof(product)))
    {
      if(pr.retpno()!=order[count])
      {
	cout<<"\n\nEntered product does not exist!Try Again\n\n";
	w=0;
      }
      break;
    }
    fp.close();
    if(w==1)
    {
      cout<<"\nQuantity:";
      cin>>qty[count];
      count++;
      cout<<"\nDo You Want To Order Another Product?(y/n)";
      cin>>ch;
    }
  } while(ch=='y'||ch=='Y');

  if(w==1)
  {
    clrscr();
    cout<<"\n\n               PLACED ORDER\n\n";
    cout<<"\n-----------------------------------------------------------------\n";
    cout<<"Pr No.\tPr Name\tQuantity\tPrice\tAmount\tDiscounted Amount\n";
    cout<<"\n-----------------------------------------------------------------\n";
    for(int x=0; x<=count; x++)
    {
      fp.open("Shop.dat",ios::in||ios::binary);
      while(fp.read((char*)&pr,sizeof(product)))
      {
	if(pr.retpno()==order[x])
	{
	  amt=pr.retprice()*qty[x];
	  dis_amt=amt-(amt*pr.retdis()/100);
	  cout<<"\n"<<order[x]<<"\t"<<pr.retname()<<"\t"<<qty[x]<<"\t\t";
	  cout<<pr.retprice()<<"\t"<<amt<<"\t"<<dis_amt;
	  total+=dis_amt;
	}
      }
      fp.close();
    }
    cout<<"\n\n\nTotal amount= Rs."<<total;
    cout<<"\n\n\nAre you sure to place this order?(y/n):";
    char answ;
    cin>>answ;
    if(answ=='n')
    {
      clrscr();
      place_order();
    }
    else
    {
      clrscr();
      cout<<"\n\nTHANK YOU FOR PLACING YOUR ORDER.\n\nOrder in Process\n\n\n";
    }
  }
}

//********* To check the Username for Admin**********
void check_username()
{
  int a, b;
  a=814;
  cout<<"\nPlease Enter the LOGIN NUMBER : ";
  cin>>b;
  if (a==b)
  {
    void admin_menu();
  }
  else
  {
    cout<<"SORRY!!! YOU ENTERED THE WRONG USERNAME";
    exit(0);
  }
}

//******Admin Menu****

void admin_menu()
{
  char opt;
  cout<<"\n\n\tADMIN MENU\n";
  cout<<"\n1.CREATE PRODUCT\n";
  cout<<"\n2.PRODUCT QUERY\n";
  cout<<"\n3.MODIFY PRODUCT\n";
  cout<<"\n4.DELETE PRODUCT\n";
  cout<<"\n5.VIEW PRODUCT MENU\n";
  cout<<"\n6.EXIT\n";
  cout<<"\n\nPlease Enter Your Choice(1-6):";
  cin>>opt;
  switch(opt)
  {
    case '1': clrscr();
	      write_product();
	      getch();
	      break;

    case '2': clrscr();
	      int num;
	      cout<<"\nEnter the Product Number:";
	      cin>>num;
	      display_sp(num);
	      getch();
	      break;

    case '3': clrscr();
	      menu();
	      modify_product();
	      getch();
	      break;

    case '4': clrscr();
	      menu();
	      delete_product();
	      getch();
	      break;

    case '5': clrscr();
	      menu();
	      cout<<"\n\nPress ANY KEY to continue.";
	      getch();
	      break;

    case '6': exit(0);
	      break;

    default: admin_menu();
  }
  clrscr();
  admin_menu();
}

//********MAIN FUNCTION*****

void main()
{
  clrscr();
  cout<<"==========================================================\n";
  cout<<"                     WELCOME\n\n";
  cout<<"                       TO\n\n";
  cout<<"                  DAY TO DAY NEEDS\n";
  cout<<"==========================================================\n";
  char ch;
  do
  {
    cout<<"\n\n\tMAIN MENU\n";
    cout<<"\n\n1.Customer\n";
    cout<<"\n2.Administrator\n";
    cout<<"\n3.Exit\n";
    cout<<"\n\nSelect Your Option(1-3):";
    cin>>ch;
    switch(ch)
    {
      case '1': clrscr();
		place_order();
		break;

      case '2': clrscr();
		check_username();
		admin_menu();
		break;

      case '3': exit(0);

      default : cout<<"PLEASE ENTER ANOTHER CHOICE";

    }
  } while(ch!=3);
}
