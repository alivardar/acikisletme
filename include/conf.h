#include <libintl.h>
#include <locale.h>

#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>

#include <glib.h>

#include <crypt.h>

#include <math.h>

#include <curses.h>
#include <form.h>
#include <menu.h>

#include <libpq-fe.h>
#include <postgres_ext.h>

//hata satirini verir
#define ERROR(s) error(s, __FILE__, __LINE__)
#define _(str) gettext(str)

#define baslik _("                        Acik isletme release 2.1 (2001-2005)                ")
#define mark " -> "

#define sql_kullanim_hatasi _("Acikisletme not found. Please control connection and database.!!!")
#define sql_sorgulama_hatasi _("Sql query can not be worked.!!!")
#define yeni_kayit_istiyormusunuz _("Record successfully saved. Do you want a new record.?")
#define yeni_kayit _("Do you want a new record.?")
#define kayit_silme_onayla _("Record will be delete. Do you want.???")
#define duzenleme_basarili _("Edit process succesfully saved.")
#define cikis_onay _("  Are you sure want to quit.?")

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

#define yatay 1
#define dikey 0


//Postgresql degiskenleri
int pgsql_field_miktar;
GList *pgsql_sonuc=NULL;
long int pgsql_kayit_miktar;
int pgsql_row_miktar;

char donen[200];


//sql server bilgileri
char sql_server[40];
char sql_kullanici[20];
char sql_sifre[20];

char sql_baglanti_dizesi[200];


/*kullanici haklari*/
char kullanici[10];
char sifre[100];
char haklar[64];
char giris_hakki=0; //0 giremez 1 girer

/*yazýcý bilgileri*/
char yazici_tipi[50];
char aygit[50];

char query[2000];

char *secilen;

//normal zaman bilgileri
time_t time_of_day;
char ay[80];
char gun[1];
char yil[3];
char saat[1];
char dakika[80];
char saniye[1];
char sifir[4] = "0";

//mysql tarih bilgileri
char sql_ay[1];
char sql_gun[1];
char sql_yil[3];

unsigned long int row_no;
char rowno[10];

/*parasal cevrimler*/
char yazi[100];
char para[100];

/*arama ekraný*/
char giris[15];

int donen_deger;

/*þifreleme*/
unsigned long seed[2];
char salt[] = "$1$........";
const char *const seedchars =
    "./0123456789ABCDEFGHIJKLMNOPQRST"
    "UVWXYZabcdefghijklmnopqrstuvwxyz";
char *password;

/*yazici*/
FILE *yazici;

WINDOW *ana_win;
WINDOW *temizle_win;

int i,j,k;

div_t bolme;

unsigned long long int kayit_no_gelen;

//yazi ile yazmak için
char yazi_deger[200];
char para_yazi[200];

//saat ve zaman alarm için
int yardim_aktif=0;
WINDOW *yardim_win;


//raporlar için deðiþkenler
char alan1[100];
char alan2[100];
char alan3[100];
char alan4[100];
char alan5[100];
char alan6[100];
char alan7[100];
char alan8[100];
char alan9[100];
char alan10[100];
char alan11[100];

// hd serial number
char serial_number[20];


/****************************************************************/

int print_in_middle (WINDOW * win, int starty, int startx, int width,
		 char *string, chtype color)
{
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx (win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 80;

  length = strlen (string);
  temp = (width - length) / 2;
  x = startx + (int) temp;
  wattron (win, color);
  mvwprintw (win, y, x, "%s", string);
  wattroff (win, color);
  refresh ();
}
/********************************************************/

int *mesaj (char *mesaj)
{  
  int c;  
  WINDOW *my_menu_win;
  int i;
  char *yaz[50];

  my_menu_win = newwin (10, 60, 10, 10);
  keypad (my_menu_win, TRUE);

  box (my_menu_win, 0, 0);
    
  print_in_middle (my_menu_win, 1, 17, 22, _("- Acik isletme info -"), COLOR_PAIR (1));
  print_in_middle (my_menu_win, 7, 25, 8, _("   OK   "), COLOR_PAIR (2));
	
  wrefresh(my_menu_win);	
  /*ekrana yaz*/
	
	if (strlen(mesaj)<50) {	mvwprintw (my_menu_win, 3, 1, "%s", mesaj);}
	
	if (strlen(mesaj)>50 && strlen(mesaj)<110)
		{		
		i=0;while (i<50) {yaz[i]=' ';i++;}
		strncpy(yaz, mesaj, 50);
		mvwprintw (my_menu_win, 3, 1, " %s", yaz);		
		i=0;while (i<51) {yaz[i]=' ';i++;}			
		strncpy(yaz, &mesaj[50], strlen(mesaj)-50 );				
		mvwprintw (my_menu_win, 4, 1, "   %s", yaz);	
		}
	
	if (strlen(mesaj)>100 && strlen(mesaj)<150)
		{
		i=0;while (i<50) {yaz[i]=' ';i++;}
		strncpy(yaz, mesaj, 50);
		mvwprintw (my_menu_win, 3, 1, " %s", yaz);		
		i=0;while (i<51) {yaz[i]=' ';i++;}
		strncpy(yaz, &mesaj[50], 50 );				
		mvwprintw (my_menu_win, 4, 1, "   %s", yaz);
		i=0;while (i<51) {yaz[i]=' ';i++;}
		strncpy(yaz, &mesaj[100], strlen(mesaj)-50 );				
		mvwprintw (my_menu_win, 5, 1, "   %s", yaz);				
		}
 
		refresh;
		
  while ((c = wgetch (my_menu_win)) != 10)
    {
      //hic bir iþlem yapýlamayacak
    }
	
  //delwin(my_menu_win);
  endwin ();

}


/****************************************************************/
char *aranacak_kelime (char msj[])
{		
//int c,ch;	
WINDOW *my_form_win;
int i;
//int *deger;
char *yaz[50];
//FORM  *my_form;	
	
my_form_win = newwin(10,60,10,10);
keypad(my_form_win, TRUE);

box(my_form_win, 0, 0);

for (i=0;i<16;i++) giris[i]=' ';

print_in_middle (my_form_win, 1, 17, 22, _("- Acik isletme Search -"), COLOR_PAIR (1));
print_in_middle (my_form_win, 7, 25, 8, "   OK   ", COLOR_PAIR (2));

	if (strlen(msj)<50){mvwprintw (my_form_win, 5, 1, "  %s", msj);	}
	
	if (strlen(msj)>50 && strlen(msj)<110)
		{		
		i=0;while (i<50) {yaz[i]=' ';i++;}
		strncpy(yaz, msj, 50);
		mvwprintw (my_form_win, 4, 1, " %s", yaz);		
		i=0;while (i<51) {yaz[i]=' ';i++;}			
		strncpy(yaz, &msj[50], strlen(msj)-50 );				
		mvwprintw (my_form_win, 5, 1, "   %s", yaz);	
		}
	
	if (strlen(msj)>100 && strlen(msj)<150)
		{
		i=0;while (i<50) {yaz[i]=' ';i++;}
		strncpy(yaz, msj, 50);
		mvwprintw (my_form_win, 4, 1, " %s", yaz);		
		i=0;while (i<51) {yaz[i]=' ';i++;}
		strncpy(yaz, &msj[50], 50 );				
		mvwprintw (my_form_win, 5, 1, "   %s", yaz);
		i=0;while (i<51) {yaz[i]=' ';i++;}
		strncpy(yaz, &msj[100], strlen(msj)-50 );				
		mvwprintw (my_form_win, 6, 1, "   %s", yaz);				
		}
	    
		
mvwprintw(my_form_win,3, 4, _("Search word    : [                                ]") );
wrefresh(my_form_win);
refresh;		

echo();
mvwscanw(my_form_win,3,24,"%15s",giris);
noecho();
		
endwin();
		
return giris;

}




/************************************************************/


char *yazi_cevir(char basamak_yazi[2])
{
char yazi_gec[100]="";
char deger[5];
char *para_bir[10]={"", _("One"), _("Two"), _("Three"), _("Four"), _("Five"), _("Six"), _("Seven"), _("Eight"), _("Nine") };
char *para_on[10]={"", _("Ten"), _("Twelve"), _("Thirty"), _("Fourty"), _("Fifty"), _("Sixty"), _("Seventy"), _("Eighty"), _("Nineteen") };
int miktar;

//birler
kopyala(deger, basamak_yazi, 2, 1);
miktar=atoi(deger);
sprintf(yazi_gec,"%s", para_bir[miktar]);
strcpy(yazi_deger, yazi_gec);

//onlar
kopyala(deger, basamak_yazi, 1, 1);
miktar=atoi(deger);
sprintf(yazi_gec,"%s%s", para_on[miktar], yazi_deger);
strcpy(yazi_deger, yazi_gec);

//yuzler
kopyala(deger, basamak_yazi,0 ,1);
miktar=atoi(deger);
if (miktar==1) sprintf(yazi_gec, _("hundert%s"), yazi_deger);  
if (miktar>1)  sprintf(yazi_gec, _("%shundert%s"), para_bir[miktar], yazi_deger ); 

strcpy(yazi_deger, yazi_gec);

return yazi_deger;
}


/**********************************************************************************/

int tarih_ayarla ()
{
  time_of_day = time (NULL);
  strftime (ay, 80, "%m", localtime (&time_of_day));
  strftime (gun, 80, "%d", localtime (&time_of_day));
  strftime (yil, 80, "%Y", localtime (&time_of_day));

  strftime (saat, 80, "%H", localtime (&time_of_day));
  strftime (dakika, 80, "%M", localtime (&time_of_day));
  strftime (saniye, 80, "%S", localtime (&time_of_day));
}

/****************************************************************/

int baslik_goruntule ()
{
touchwin(temizle_win);
wrefresh(temizle_win);	
attron (COLOR_PAIR (2));
mvprintw (0, 0, baslik);
mvprintw (LINES-1 , 0, baslik);
attroff (COLOR_PAIR (2));
wrefresh(temizle_win);
refresh;
}

/********************************************************/

int pgsql_sorgula(char query_string[2000]) 
{
char        state_code[3];
PGconn     *conn;
PGresult   *res;
int         i,j;
long int satir_sayisi;
char        cmd[2000];

//test amacýyla query dosya icine yazma
//sprintf(cmd,"echo '%s' >> /lisletme.sql\n", query_string);
//system(cmd);

pgsql_sonuc=NULL;
conn = PQconnectdb(sql_baglanti_dizesi);
	
if (PQstatus(conn) == CONNECTION_BAD)
{
mesaj( _("Sql connection not found. Please be sure your configuration. !!! ") );
return 1;
}

//alltaki satir sadece tarih formatýný avrupa yapmak ýcýn
if ( (strstr(getenv("LANG"), "de")) || (strstr(getenv("LANG"), "tr")) ) PQexec(conn, "set datestyle='German' " );
else PQexec(conn, "set datestyle='US' " );
		
res = PQexec(conn, query_string);
if  (PQresultStatus(res) != PGRES_TUPLES_OK) 
{
	if (PQresultStatus(res) !=  PGRES_COMMAND_OK) 
	{	
	PQclear(res);
	PQfinish(conn);
//        mesaj("error");
	return 1;
	}
}


//field sayisini ogren
pgsql_field_miktar = PQnfields(res);
pgsql_row_miktar = PQntuples(res);
/* //field isimleri ekrana basilabilir
for (i = 0; i < pgsql_field_miktar; i++)
printf("%-15s", PQfname(res, i));
printf("\n\n");
*/

for (i = 0; i < pgsql_row_miktar; i++)
{
	for (j = 0; j <	pgsql_field_miktar; j++)
	{			
		pgsql_sonuc=g_list_append(pgsql_sonuc, g_strdup( PQgetvalue(res, i, j)  ) );		
	}
}
//eger sýfýr ise basarili olarak çalýþmýstýr ancak deger donmez
//ornek olarak insert, update veyahutta tablo bostur ozaman her ihtimale karsi filed kadar
//dongu yapýp yzaalým
if (i==0) 
{
	for (j = 0; j <	pgsql_field_miktar; j++)
	{			
	pgsql_sonuc=g_list_append(pgsql_sonuc,  " " );		
	}
	PQclear(res);PQfinish(conn);return 0;
}
	pgsql_sonuc=g_list_nth(pgsql_sonuc, 0);
	//satir_sayisi=g_list_length(pgsql_sonuc);

	//printf("%i, %i, %i",pgsql_row_miktar,satir_sayisi,pgsql_field_miktar);

PQclear(res);
PQfinish(conn);
         
return 0;
}


/********************************************************/

char *pgsql_kayit_field()
{
if (pgsql_sonuc==NULL)
{
sprintf(donen,"");
return donen;
}

sprintf(donen,"%s",(gchar *)pgsql_sonuc->data );

//pgsql_sonuc=pgsql_sonuc->next;
pgsql_sonuc=g_list_next(pgsql_sonuc);
return g_strstrip(donen);
}

/********************************************************/
int *row_no_buyuk (char *tablo)
{  
  sprintf (query, "select max(row_no) from %s", tablo);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

row_no = atoi ( pgsql_kayit_field() );
row_no = row_no + 1;
sprintf (rowno, "%i", row_no);

}
/********************************************************/


int *row_no_kucuk (char *tablo)
{
sprintf (query, "select min(row_no) from %s", tablo);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
row_no = atoi ( pgsql_kayit_field() );	
//row_no = row_no + 1;
sprintf (rowno, "%i", row_no);
	
}

/********************************************************/
int *row_no_buyuk_tur (char *tablo,char tur[5])
{

sprintf (query, "select max(row_no) from %s where tur='%s'", tablo, tur);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
row_no = atoi ( pgsql_kayit_field() );
row_no = row_no + 1;
sprintf (rowno, "%i", row_no);
    
}
/********************************************************/


int *row_no_kucuk_tur (char *tablo, char tur[5])
{

sprintf (query, "select min(row_no) from %s where tur='%s'", tablo, tur);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
row_no = atoi ( pgsql_kayit_field() );
sprintf (rowno, "%i", row_no);
	
}

char *rtrim (char *string, char const *set)
{
  register char *s;
  register int len;

  if (!set || '\0' == *set || !string || '\0' == *string)
    return (string);
  len = strlen (string);

  for (s = &string[len - 1]; len > 0; s--, len--)
    {
      if (!strrchr (set, *s))
	{
	  break;
	}
    }
  *(++s) = '\0';

  return (string);
}

/********************************************************/


int *onayla (char *mesaj)
{
const char *secenek[] = {  "  Yes  ", "   No   ",  (char *) NULL, };

  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;
  int *deger;
  char *yaz[50];
	
  n_choices = ARRAY_SIZE (secenek);
  my_items = (ITEM **) calloc (n_choices, sizeof (ITEM *));
  for (i = 0; i < n_choices; ++i)
    my_items[i] = new_item (secenek[i], " ");

  my_menu = new_menu ((ITEM **) my_items);

  menu_opts_off (my_menu, O_SHOWDESC);
  my_menu_win = newwin (10, 60, 10, 10);
  keypad (my_menu_win, TRUE);

  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 2, 20, 7, 20));
  set_menu_format (my_menu, 1, 2);

  set_menu_mark (my_menu, "");

print_in_middle (my_menu_win, 1, 20, 22, _("- Acik isletme Question -"), COLOR_PAIR (1));
    
  box (my_menu_win, 0, 0);
  
/*ekrana yaz*/
		
	if (strlen(mesaj)<50){ 	mvwprintw (my_menu_win, 4, 1, "  %s", mesaj); }
		
	if (strlen(mesaj)>50 && strlen(mesaj)<100)
		{
		i=0;while (i<50) {yaz[i]=' ';i++;}
		strncpy(yaz, mesaj, 50);
		mvwprintw (my_menu_win, 4, 1, " %s", yaz);		
		i=0;while (i<50) {yaz[i]=' ';i++;}			
		strncpy(yaz, &mesaj[50], strlen(mesaj)-50 );				
		mvwprintw (my_menu_win, 5, 1, "   %s", yaz);	
		}
	  
  post_menu (my_menu);
  wrefresh (my_menu_win);

/*ilk deger */
  deger = 1;

  while ((c = wgetch (my_menu_win)) != 10)
    {
      switch (c)
	{
	case KEY_LEFT:
	  menu_driver (my_menu, REQ_LEFT_ITEM);
	  deger = 1;
	  break;
	case KEY_RIGHT:
	  menu_driver (my_menu, REQ_RIGHT_ITEM);
	  deger = 0;
	  break;
	}
      wrefresh (my_menu_win);
    }
  unpost_menu (my_menu);
  free_menu (my_menu);
  for (i = 0; i < n_choices; ++i)
    free_item (my_items[i]);
  endwin ();

  return deger;
}


/***************************************************************/

char *field_goster (char *tablo, char *field, int kayit_no)
{

  ITEM *my_items[20];
  ITEM *cur_item;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;  
  char *deger[20][50];
  char donen[50];

row_no_buyuk(tablo);
if (row_no<kayit_no) kayit_no=kayit_no-15;

  sprintf (query,
"select %s from %s order by %s limit 15 offset %d",
  field, tablo,field, kayit_no);
	   
if ( strlen( g_strstrip(giris)) >2) sprintf (query,
"select %s from %s where %s like '%%%s%%' order by %s limit 15 offset 0",
field, tablo, field, giris, field );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
for (i=0;i<pgsql_row_miktar;i++)
	{
	sprintf (deger[i], "%-60.60s", pgsql_kayit_field() );
	my_items[i] = new_item (&deger[i], " ");
	}
	
  my_items[i] = (ITEM *) NULL;
  my_menu = new_menu ((ITEM **) my_items);

  my_menu_win = newwin (21, 75, 1, 3);

  keypad (my_menu_win, TRUE);

  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 16, 70, 3, 1));
  set_menu_format (my_menu, 15, 1);

  set_menu_mark (my_menu, mark);

  box (my_menu_win, 0, 0);
  
  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, 73);
  mvwaddch (my_menu_win, 2, 74, ACS_RTEE);
  
 print_in_middle (my_menu_win, 1, 0, 75,
	  _("          F5-> Search      ESC-> Exit       Enter-> Select      "), COLOR_PAIR (1));
print_in_middle(my_menu_win, 20, 0, 65, _("  PAGE UP -> Prior Page     PAGE DOWN -> Next Page "), COLOR_PAIR(1));
	
  post_menu (my_menu);
  wrefresh (my_menu_win);

  menu_driver (my_menu, REQ_DOWN_ITEM);
  cur_item = current_item (my_menu);
  sprintf (donen, "%s", item_name (cur_item));
  wrefresh (my_menu_win);

  while ((c = wgetch (my_menu_win)) )
    {
      switch (c)
		{
		case 10:
			cur_item = current_item (my_menu);
			sprintf (donen, "%s", item_name (cur_item));				
			return g_strstrip(donen);
			
		case KEY_DOWN:
			  menu_driver (my_menu, REQ_DOWN_ITEM);
			  break;
		case KEY_UP:
			  menu_driver (my_menu, REQ_UP_ITEM);		
			  break;
		case KEY_NPAGE:
			kayit_no=kayit_no+15;
			cur_item = current_item (my_menu);
			sprintf (donen, "%s", field_goster (tablo, field, kayit_no));
							
			return donen;	  
						
		case KEY_PPAGE:
				if (kayit_no>14) {kayit_no=kayit_no-15;}
				cur_item = current_item (my_menu);
				sprintf (donen, "%s", field_goster (tablo, field, kayit_no));
				
				return donen;	  
				
		case KEY_F (5):	/*arama tusu basýlýrsa */
				aranacak_kelime( _("Arama için bir deðer giriniz. Düzgün sýralama için boþ býrakýnýz. ") ) ;
				field_goster (tablo, field, 0);
				return donen;	
	
		}
		cur_item = current_item (my_menu);		  
		wrefresh (my_menu_win);		
      }
	  

}

/**************************************************************/
int ayar_dosyasi_oku ()
{
  FILE *dosya;
  char satir[50];
  char dizi[50];

  dosya = fopen ("/opt/acikisletme/conf/psql.conf", "r");

  if (dosya == NULL)
    {
      mesaj ("/opt/acikisletme/conf/psql.conf file not found.!!!");
      exit (1);
    }

  while (!feof (dosya))
    {
      fgets (satir, 50, dosya);
      if (strcmp (satir, "#") != -1)
	{
	  //mysql_server ismi okunuyor
	  if (strstr (satir, "sql_server"))
	    {
	      strcpy (dizi, &satir[12]);
	      rtrim (dizi, "\n");
	      rtrim (dizi, " ");
	      strcpy (sql_server, dizi);
	    }
	  //kullanici adi okunuyor
	  if (strstr (satir, "kullanici"))
	    {
	      strcpy (dizi, &satir[10]);
	      rtrim (dizi, "\n");
	      rtrim (dizi, " ");
	      strcpy (sql_kullanici, dizi);
	    }
	  //sifre okunuyor
	  if (strstr (satir, "sifre"))
	    {
	      strcpy (dizi, &satir[6]);
	      rtrim (dizi, "\n");
	      rtrim (dizi, " ");
	      strcpy (sql_sifre, dizi);
	    }
	}

    }
  fclose (dosya);
	
if ( strlen(g_strstrip(sql_sifre))>1 )
sprintf(sql_baglanti_dizesi,"dbname=acikisletme user=%s password=%s host=%s",
sql_kullanici, sql_sifre, sql_server);
else
sprintf(sql_baglanti_dizesi,"dbname=acikisletme user=%s host=%s",
sql_kullanici, sql_server);
	
//printf("mysql_server:|%s|\n", sql_server);
//printf("kullanici:|%s|\n", sql_kullanici);
//printf("sifre:|%s|\n", sql_sifre);
//refresh;		


//çýkýþ
}

/**************************************************************************/
int kopyala (char bir[], char iki[], int nerden, int miktar)
{
  int i = 0;
  while (i < miktar)
    {
      bir[i] = iki[nerden + i];
      i++;
    }
}
/************************************************************/

char *yazidan_paraya_cevir (char yazi[])
{
  int k;
  div_t bolme;
  char okunan[10] = "";

  strcpy (para, "");
  bolme = div (strlen (yazi), 3);

  kopyala (okunan, yazi, 0, bolme.rem);
  strcat (para, okunan);

  if (bolme.rem > 0) strcat (para, ".");

  for (k = 0; k < bolme.quot + 1; k++)
    {
      kopyala (okunan, yazi, (k * 3) + bolme.rem, 3);
      if (strcmp (okunan, ""))
	{
	  strcat (para, okunan);
	  if (k + 1 != bolme.quot)  strcat (para, ".");
	}
    }
  //strcat (para, "TL");

  return g_strstrip(para);
}


/*************************************************************/

char *paradan_yaziya_cevir (char para[])
{
  char *parca = "";

  strcpy (yazi, "");
  parca = strtok (para, ".TL$EURYTL");
  while (parca != NULL)
    {
      strcat (yazi, parca);
      parca = strtok (NULL, ".TL$EURYTL");
    }

  return g_strstrip(yazi);
}

/************************************************************/

int sql_tarih_ayir (char tarih[])
{  
//mesaj(tarih);
  kopyala (sql_yil, tarih, 6, 4);
  kopyala (sql_ay, tarih, 3, 2);
  kopyala (sql_gun, tarih, 0, 2);
}

/************************************************************/
int terminal_kontrol_et ()
{
  char terminal[20];

  strcpy (terminal, getenv ("TERM"));
  if (strcmp (terminal, "linux"))
    mesaj ( _(" Açýkiþletme'yi konsol haricinde calistirmak       problemlere yol acmaktadir.    Uygulamanin ekranlarinda problem yaratmaktadir.  Uygulamayi konsol altinda kullaniniz") );
}

/************************************************************/

int sonlandir (int sig)
{
mesaj( _("Lütfen menüden çýkýþ bölümünü kullanýnýz.!!!      Uygulamada veri kaybýna neden olabilirsiniz.       Uygulamadan çýkýlýyor....") );
exit(0);	
}

/************************************************************/

int stok_miktar_isle (char *malzeme_kodu, int miktar, int tip)
{
long long int mevcut;
char mevcut_str[50];
int i;
	
sprintf (query,"select * from stokkart where malzeme_kodu='%s'", malzeme_kodu);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
for (i=0;i<6;i++) pgsql_kayit_field();
sprintf(mevcut_str,"%s", pgsql_kayit_field() );
			
mevcut=atoll(mevcut_str);

//azaltmami yapilacak
if (tip==-1) mevcut=mevcut-miktar;
//çogaltmami yapilacak
if (tip==1) mevcut=mevcut+miktar;

/*tekrar sql icine yazilir*/
sprintf(mevcut_str,"%lli", mevcut );

sprintf (query,"update stokkart set mevcut='%s' where malzeme_kodu='%s'", mevcut_str, malzeme_kodu);

if ( pgsql_sorgula(query) ==1 )	{ mesaj(sql_sorgulama_hatasi); return 1;}

/*
hata ise -1 gider
normal ise 0 gider
*/
return 0;
}



/************************************************************/


int cari_miktar_isle (char firma_adi[], long long int miktar, int tip)
{
/*tip -1 ise tediye makbuzu 1 ise tahsilat makbuzu kabul edilir*/
	
/*firmalar tablosunda bakiye ve durum deðiþecektir*/
	
long long int mevcut;
char mevcut_str[50];
char durum[5];
	
sprintf (query,"select bakiye, durum from firmalar where firma_adi like '%%%s%%'", firma_adi );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

sprintf(mevcut_str,"%s", pgsql_kayit_field() );
sprintf(durum,"%s", pgsql_kayit_field() );

mevcut=atoll(mevcut_str);

if (strlen(durum)<1) {if (tip==1) strcpy(durum,"B"); if (tip==-1) strcpy(durum, "A");}

	
	
//tahsil ettigimizde ve evvelden borclu ise
if  ( strstr(durum,"B") && (tip==1) )
{
mevcut=mevcut-miktar;
if (mevcut<0) { strcpy(durum,"A"); mevcut=abs(mevcut); }
}

//tediye yapildigi zaman borclu ise
if  ( strstr(durum,"B") && (tip==-1) )
{
mevcut=mevcut+miktar;
}

//alacaklý iken tekrar alacaklý olursa
if  ( strstr(durum,"A") && (tip==1) )
{
mevcut=mevcut+miktar;
}


//yeni acilmis bir hesap ise tahsil veya tediye yapilmis ise
if ( strstr(g_strstrip(durum), " ") && ( (tip==1) || (tip==-1) ) )
{
mevcut=mevcut+miktar;
if (tip==1) { strcpy(durum,"A");}
if (tip==-1) { strcpy(durum,"B");}
}

/************************************************/

//tediye yapildigi zaman borclu olmasi durumunda
if  ( strstr(durum,"B") && (tip==-1) )
{
mevcut=mevcut+miktar;
}

//tediye yapildigi zaman alacakli olmasi durumunda
if ( strstr(durum,"A") && (tip==-1) )
{
mevcut=mevcut-miktar;
if (mevcut<0) {strcpy(durum,"B"); mevcut=abs(mevcut);}
}

/*tekrar sql icine yazilir*/
sprintf(mevcut_str,"%lli", mevcut );

sprintf (query,"update firmalar set bakiye='%s', durum='%s' where firma_adi like '%%%s%%'", mevcut_str, durum, firma_adi);

if ( pgsql_sorgula(query) ==1 )	{ mesaj(sql_sorgulama_hatasi); return 1;}

/*
hata ise -1 gider
normal ise 0 gider
*/
	
}

/************************************************************/

int haklari_kontrol_et(int izin_konum)
{
/*
11111 =5 hane su an için	
stok grisinmi	=0
cari girsinmi	=1
rapor girsnmi	=2
silme yapsinmi	=3
yeni kayit yapsinmi	=4
*/
	giris_hakki=0;
	
	switch (izin_konum)
		{
		case 0:
		if (haklar[0]=='1') giris_hakki=1;
		break;
		
		case 1:
		if (haklar[1]=='1') giris_hakki=1;
		break;
		
		case 2:
		if (haklar[2]=='1') giris_hakki=1;
		break;
		
		case 3:
		if (haklar[3]=='1') giris_hakki=1;
		break;
		
		case 4:
		if (haklar[4]=='1') giris_hakki=1;
		break;
	
		}
	
return giris_hakki;

}//fonksiyon sonu


/************************************************************/

char *sifre_kripto_yap(char *text)
{
 int i;
 seed[0] = time(NULL);
 seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);
  
 for (i = 0; i < 8; i++)
    salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];
 
 password = crypt(text, salt);
 
 return password;	
}

/************************************************************/

int sifre_kripto_coz(char *yazi, char *sifreli)
{
  char *result;
  int donen;
	
  result = crypt(yazi, sifreli);
  donen = strcmp (result, sifreli)==0;
  //puts(ok ? _("Access granted.") : _("Access denied.") );
	
  return donen ? 0 : 1;
	
}

/************************************************************/

int yedek_kontrol_et ()
{
  FILE *dosya;
  char deger[300];
  char satir[50];
  char dizi[50];
  char yazi[50];
  char otoyedek[50];
  char yedektar[50];

/*eger root degil ise yedek alma*/
if (getuid()>0) return;

  /*oncelikle var olan ayar dosyasý okunacak */
  dosya = fopen ("/opt/acikisletme/conf/yedek.conf", "r");

if (dosya == 0) { printf("\n/opt/acikisletme/conf/yedek.conf dosyasý açýlamadý.\n");return 0;}

  while ( !(feof(dosya)) )
    {
      fgets (satir, 50, dosya);
      if (strcmp (satir, "#") != -1)
	{
	  //hergun otoyedek alsýnmý
	  if (strstr (satir, "otoyedek"))
	    {
	      strcpy (dizi, &satir[7]);
	      rtrim (dizi, "\n");
	      rtrim (dizi, " ");
	      sprintf (otoyedek, "%s", dizi);
	    }
	  //en son yedekleme tarihi
	  if (strstr (satir, "yedektar"))
	    {
	      strcpy (dizi, &satir[9]);
	      rtrim (dizi, "\n");
	      rtrim (dizi, " ");
	      sprintf (yedektar, "%s", dizi);
	    }
	}
    }
  fclose (dosya);

  /*eðer her gun otoyedek al deðeri true ise devam flase ise çýk */
  if (strstr (otoyedek, "false"))   return;

  /*tarih kontrolu yapýlacaktýr eðer en son yedek alýnan tarih bugun ise yedekleme yapma ve çýkýlacak */
  tarih_ayarla ();
  /*tarih kontrolu */
  /*eðer daha eski ise yedekleme  yapýlacak */

  //postgresql yedekleme
  //kullanici adi , sifre ,hopst makine bilgileri alinmali  
  
  ayar_dosyasi_oku ();
  
  sprintf (yedektar, "%s.%s.%s", gun, ay, yil);
  if ( (strlen(sql_sifre)<2) || (strstr(sql_sifre, "")) )
  sprintf (deger, "pg_dump --username=%s --host=%s --file=../backup/%s.sql isletme",
  sql_kullanici, sql_server, yedektar);
  else
  sprintf (deger, "pg_dump --username=%s --password=%s --host=%s --file=../backup/%s.sql isletme ",
  sql_kullanici, sql_sifre, sql_server, yedektar);
  
  system (deger);

  dosya = fopen ("/opt/acikisletme/conf/yedek.conf", "w");

  //otoyedek degiskeni yazýldý true false
  sprintf (deger, "otoyedek=true\n");
  fputs (deger, dosya);

  //yedektar icine en son yedek alýnan tarih yazýldý
  sprintf (deger, "yedektar=%s\n", yedektar);
  fputs (deger, dosya);

  fclose (dosya);

return ;

}


/************************************************************/

int ekran_temizle()
{
erase();
refresh();
}

/*************************************************************/
// printf("\n%s\n", para_yazdir("111222333444666777") );
//ornek kullaným

char *para_yazdir(char para_str[30])
{
char yazi[200];
char yazi_gec[200];
char deger3[2];
int uzun;

uzun=strlen(para_str);

//birler
kopyala(deger3, para_str, uzun-3,3);
sprintf(yazi, "%s", yazi_cevir(deger3));

//binler
if (uzun>3){
kopyala(deger3, para_str, uzun-6,3);
if (!strstr(deger3, "000") )
{sprintf(yazi_gec, _("%sbin%s"), yazi_cevir(deger3), yazi);  strcpy(yazi, yazi_gec);}
}

//milyonlar
if (uzun>6){
kopyala(deger3, para_str, uzun-9,3);
if (!strstr(deger3, "000") )
{sprintf(yazi_gec, _("%smilyon%s"), yazi_cevir(deger3), yazi);  strcpy(yazi, yazi_gec);}
}

//milyarlar
if (uzun>9){
kopyala(deger3, para_str, uzun-12,3);
if (!strstr(deger3, "000") )
{sprintf(yazi_gec, _("%smilyar%s"), yazi_cevir(deger3), yazi);  strcpy(yazi, yazi_gec);}
}

//trilyon
if (uzun>12){
kopyala(deger3, para_str, uzun-15,3);
if (!strstr(deger3, "000") )
{sprintf(yazi_gec, _("%strilyonr%s"), yazi_cevir(deger3), yazi);  strcpy(yazi, yazi_gec);}
}

//katrilyon
if (uzun>15){
kopyala(deger3, para_str, uzun-18,3);
sprintf(yazi_gec, _("%skatrilyonr%s"), yazi_cevir(deger3), yazi);  
strcpy(yazi, yazi_gec);
}

strcpy(para_yazi, yazi);

return para_yazi;
}


/********************************************************************/
int zaman_yazici()
{
if (yardim_aktif==0) {return 0;}
tarih_ayarla();
mvwprintw(yardim_win, 3,20, _("Saat: %s:%s:%s      Tarih: %s/%s/%s"), saat, dakika, saniye, gun, ay, yil);
touchwin(yardim_win);
wrefresh(yardim_win);
}
/********************************************************************/

int tus_yardim() 
{
int c;
//timer içi gerekli 
struct itimerval timer;
	
yardim_aktif=1;
yardim_win = newwin(24, 80, 0, 0);
box(yardim_win, 0, 0);

attron(A_BOLD);
mvwprintw(yardim_win, 5,20, _("Açýk Ýþletme Klavye Kullaným Klavuzu") );
mvwprintw(yardim_win, 7, 25, _("Kullanýlabilecek Tuþlar") );
attroff(A_BOLD);

mvwprintw(yardim_win, 9, 1,  _("* F1    - Yardým ekraný") );
mvwprintw(yardim_win, 10, 1, _("* F2    - Yeni kayýt açýlmasý durumunda kayýt tuþu") );
mvwprintw(yardim_win, 11, 1, _("* F3    - Ekranlarda yeni kayýt açýlmasýný saðlar") );
mvwprintw(yardim_win, 12, 1, _("* F4    - Üzerinde bulunulan kaydý siler") );
mvwprintw(yardim_win, 13, 1, _("* F8    - Üzerinde bulunulan girdi konumunda seçenekleri gösterir") );
mvwprintw(yardim_win, 14, 1, _("* Tab   - Önceki giriþ alanýna konumlanýr.") );
mvwprintw(yardim_win, 15, 1, _("* Enter - sonraki giriþ alanýna konumlanýr.") );
mvwprintw(yardim_win, 20, 1, _("* F10/ESC -> Menülerde çýkýþ için kullanýlabilir.") );

mvwprintw(yardim_win, 18, 1, _("Ayrýca kayýtlar arasýnda Page-Up/Page Down tuþlarý ile önceki ve sonraki") );
mvwprintw(yardim_win, 19, 1, _("kayýtlara hareket edilebilir. Ok tuþlarý ile kayýt giriþ alanlarýnda ") );
mvwprintw(yardim_win, 20, 1, _("istenilen alana konumlanýlabilir.") );
	
mvwprintw(yardim_win, 23, 20, _("Çýkýþ için enter tuþuna basýnýz.") );
wmove(yardim_win, 7,57);
touchwin(yardim_win);
wrefresh(yardim_win);


	/*********************/
	  signal(SIGALRM, zaman_yazici);
	  timer.it_interval.tv_sec=0;
	  timer.it_interval.tv_usec=1000;
	  timer.it_value.tv_sec=0;
	  timer.it_value.tv_usec=1000;
	  setitimer(ITIMER_REAL,&timer,NULL);
	/*********************/

    while (1) {	
	switch(wgetch(yardim_win))
	{
	case 10:		
	    yardim_aktif=0; //alarmý kapatmak için
		delwin(yardim_win);
		//çýkýþ
	    return 0;	
	}
    }
   //çýkýþ
}


/*****************************************************************************/

int irsaliye_gun_kontrol()
{
//1 donmesi durumunda 9 gun gecmis ama faturasý kesilmemis irsaliye var
//Bu yeni postgresql de calismadý
//sprintf (query,"select * from irsaliyeler where tarih<now()-8 and tur='1' and fatura_no=''");
sprintf (query,"select * from irsaliyeler where tarih<now()-'5 days'::interval and tur='1' and fatura_no=''");
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
//0 dan buyuk kayýt var ise 1 dondur aksi halde 0
if (pgsql_row_miktar>0) return 1; else return 0;

}//fonksiyon sonu

/*****************************************************************************/

int baslik_yaz()
{
attron (COLOR_PAIR (2));
mvprintw (0, 0, baslik);
mvprintw (LINES-1 , 0, baslik);
attroff (COLOR_PAIR (2));
}
/*****************************************************************************/
