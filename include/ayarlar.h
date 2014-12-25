
int ayarlar()
{

char *choices[] = {
			_("User Settings"),
			_("SQL Settings "),
			_("Firm Settings"),
			_("Printer Settings"),
			_("Backup Settings"),
			_("Bill Design Screen"),
			_("Back"),
                        (char *)NULL,
                  };

ITEM **my_items;
int ch;
MENU *my_menu;
WINDOW *my_menu_win;
int n_choices, i;
ITEM *cur_item;	

		
	baslik_goruntule();

	if (getuid()>0) 
	{
	mesaj( _("You are not root user, you must be login user root.!!!") );
	refresh();	
	return;
	}
				  
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	    
	n_choices = ARRAY_SIZE(choices);
    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
    for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i], " ");

    my_menu = new_menu((ITEM **)my_items);

    my_menu_win = newwin(15, 50, 5, 10);
    keypad(my_menu_win, TRUE);
     
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 10, 40, 4, 2));

    set_menu_mark(my_menu, mark);

    box(my_menu_win, 0, 0);
    print_in_middle(my_menu_win, 1, 0, 45, _("Configuration"), COLOR_PAIR(1));
	
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 48);
	mvwaddch(my_menu_win, 2, 49, ACS_RTEE);
        
	post_menu(my_menu);		
	wrefresh(my_menu_win);	
	touchwin(my_menu_win);


	while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
			case KEY_F(10):
				ch=27;
				break;
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;			
			case 10:
	    			cur_item = current_item(my_menu);
				    switch(item_index(cur_item) + 1)
				        {	
					case 1:	/*kullanýcý haklarý*/
						
						//bu alana giriþ için root kullancýsý hakalrý gerekir
						kullanici_onayla_ayarlar();
						
						baslik_goruntule();
					    
						touchwin(my_menu_win);
						wrefresh(my_menu_win);						
						break;
					
					case 2:	/*sql server ayarlarý*/

						sql_server_ayar();
											
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						
						break;
    					case 3:	/*Sirket bilgileri*/
						sirket_bilgileri();
					
						baslik_goruntule();

						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						
						break;					
					case 4:
						yazici_ayarlari();
						
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						
						break;
					case 5:
						yedekleme_ayarlari();
						
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						
						break;		
					case 6:
						system("mcedit /opt/acikisletme/conf/fatura.sablon");
						
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						
						break;			
					case 7:	/*ana menu donus cikis*/
								
    					for(i = 0; i < n_choices; ++i) free_item(my_items[i]);
					    
					    return ;	
   					    break;
												
					
				        }
                  break;
				
				refresh();
				pos_menu_cursor(my_menu);

		}
                wrefresh(my_menu_win);
	}	

	
	return;
    
}



/*****************************************************************/

int sql_server_ayar()
{
/*sql server kullanýcý adý ver sifre ayarlarý*/
	
	FIELD *field[3];
	FORM  *my_form2;
	FILE *dosya;
	char deger[50];
	char satir[50];
	char dizi[50];
	int ch,i;
	char hata;
	PGconn     *conn;

	baslik_goruntule();

	touchwin(ana_win);
	wrefresh(ana_win);

	field[0] = new_field(1, 30, 8, 25, 0, 0);
	field[1] = new_field(1, 30, 10, 25, 0, 0);
	field[2] = new_field(1, 30, 12, 25, 0, 0);
	field[3] = NULL;

	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	
	for (i=0;i<3;i++)
	{
	set_field_fore(field[i], COLOR_PAIR(1));
	set_field_back(field[i], COLOR_PAIR(2));
	field_opts_off(field[i], O_AUTOSKIP);
	}	
	

	my_form2 = new_form(field);
	post_form(my_form2);
	refresh();
	
	set_current_field(my_form2, field[0]); 

	mvprintw(8, 10, _("User name :") );
	mvprintw(10, 10, _("Password      :") );
	mvprintw(12, 10, _("Server Name or IP:") );

	attron(COLOR_PAIR(2));
		
	mvprintw(1, 0, _("                    PostgreSQL Server Connection Settings                      ") );
	mvprintw(LINES - 2, 0, _("           F2 -> Save     F5-> Connection Test     ESC-> Cancel              ") );
	attroff(COLOR_PAIR(2));

	mvprintw(LINES - 8, 0, _(" - Test baðlantýsýný denemeden önce kayýt ediniz.") );
	mvprintw(LINES - 7, 0, _(" - Kullanýcý adý bölümüne PostgreSQL server kullanýcý adý yazýlýr.") );
	mvprintw(LINES - 6, 0, _(" - Þifre bölümüne kullanýcý adýna ait olan þifre yazýlýr.") );
	mvprintw(LINES - 5, 0, _(" - Bilgisayar bölümüne PostgreSQL server bulunan bilgisayar adý veya ip nosu yazýlýr. ") );
	
	dosya=fopen("/opt/acikisletme/conf/psql.conf","r");
	while (!feof(dosya))
		{	
	fgets(satir, 50,  dosya);
		if (strcmp(satir,"#" )!=-1)
			{		
			//mysql_server ismi okunuyor
			if (strstr(satir,"psql_server")) {strcpy(dizi,&satir[12]);rtrim(dizi,"\n"); rtrim(dizi," ");strcpy(sql_server,dizi);  }
			//kullanici adi okunuyor
			if (strstr(satir,"kullanici")) { strcpy(dizi,&satir[10]); rtrim(dizi,"\n"); rtrim(dizi," "); strcpy(sql_kullanici, dizi); }
			//sifre okunuyor
			if (strstr(satir,"sifre")) { strcpy(dizi,&satir[6]);rtrim(dizi,"\n"); rtrim(dizi," "); strcpy(sql_sifre, dizi); }
			}
		}
	fclose(dosya);	
	
	/*okunan degerler field içlerine yerleþtirelecek*/
	set_field_buffer(field[0], 0,  sql_kullanici);
	set_field_buffer(field[1], 0,  sql_sifre);
	set_field_buffer(field[2], 0,  sql_server);
		
	refresh();

while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
		case KEY_F(10):
			ch=27;
			break;			
		case 8: /*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr*/
        	form_driver(my_form2, REQ_PREV_CHAR);
			form_driver(my_form2, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE:
			  form_driver (my_form2, REQ_PREV_CHAR);
			  form_driver (my_form2, REQ_DEL_CHAR);
			  break;	
		case 9: /*TAB tusuna basýlmasý durumunda sonraki field konumlan*/
			form_driver(my_form2, REQ_NEXT_FIELD);
			form_driver(my_form2, REQ_END_LINE);
			break;		
		case 10: /*ENTER tuþuna basýlmasý durumunda sonraki feild gececek*/
			form_driver(my_form2, REQ_NEXT_FIELD);
			form_driver(my_form2, REQ_END_LINE);
			break;		
		case KEY_DC:	/*del PGconn     *conn; PGresult   *res;tusu*/		
    		form_driver(my_form2,REQ_DEL_CHAR);
			break;			
		case KEY_HOME:
			form_driver(my_form2,REQ_BEG_LINE);
			break;						
		case KEY_END:
			form_driver(my_form2,REQ_END_LINE);
			break;
		case KEY_DOWN:
			form_driver(my_form2, REQ_NEXT_FIELD);
			form_driver(my_form2, REQ_END_LINE);
			break;
		case KEY_UP:
			form_driver(my_form2, REQ_PREV_FIELD);
			form_driver(my_form2, REQ_END_LINE);
			break;
		case KEY_LEFT:
    		form_driver(my_form2, REQ_LEFT_CHAR);
			break;
		case KEY_RIGHT:
    		form_driver(my_form2, REQ_RIGHT_CHAR);
			break;			
		case  KEY_F(2): /*kaydet*/
			form_driver(my_form2, REQ_NEXT_FIELD);
					
			dosya=fopen("/opt/acikisletme/conf/psql.conf","w");
				sprintf(deger,"psql_server=%s\n", g_strstrip(field_buffer(field[2],0) )  );
			fputs(deger,dosya);
				sprintf(deger,"kullanici=%s\n", g_strstrip(field_buffer(field[0],0) )  );
			fputs(deger,dosya);
				sprintf(deger,"sifre=%s", g_strstrip(field_buffer(field[1],0) )  );
			fputs(deger,dosya);
			fclose(dosya);
	
			mesaj( _("Settings succesfully saved.") );
			form_driver(my_form2, REQ_PREV_FIELD);
			form_driver(my_form2, REQ_NEXT_FIELD);
			refresh();
			ch=NULL;
			break;		
		case  KEY_F(5): /*test baglantýsý*/
			form_driver(my_form2, REQ_NEXT_FIELD);
			form_driver(my_form2, REQ_PREV_FIELD);			
			refresh();			
			
			ayar_dosyasi_oku ();
		
			conn = PQconnectdb(sql_baglanti_dizesi);
	
			if (PQstatus(conn) == CONNECTION_BAD)
			{
			mesaj( _("SQL server connection problem.!!!") );
			form_driver(my_form2, REQ_PREV_FIELD);
			form_driver(my_form2, REQ_NEXT_FIELD);
			refresh();
			break;
			}
			
			mesaj( _("SQL server connection is succesfully.") );
			form_driver(my_form2, REQ_PREV_FIELD);
			form_driver(my_form2, REQ_NEXT_FIELD);
			refresh();  			    
			
			break;						
		
			default:
			form_driver(my_form2, ch);
			break;
		}
	}

for (i=0;i<3;i++);free_field(field[i]);

}




/*****************************************************************/

int sirket_bilgileri()
{

char query[2000]="";
FIELD *sfield[7];
FORM  *my_form;
int ch,i;
WINDOW *sirket_win;	
	
ekran_temizle();

sirket_win = newwin(15, 70, 5, 5);
wmove(sirket_win, 7,57);

sfield[0] = new_field(1, 60, 4, 17, 0, 0);
sfield[1] = new_field(1, 60, 6, 17, 0, 0);
sfield[2] = new_field(1, 60, 8, 17, 0, 0);
sfield[3] = new_field(1, 30, 10, 17, 0, 0);
sfield[4] = new_field(1, 30, 12, 17, 0, 0);
sfield[5] = new_field(1, 30, 14, 17, 0, 0);
sfield[6] = new_field(1, 30, 16, 17, 0, 0);
sfield[7] = NULL;

for (i=0;i<7;i++)
{ 
field_opts_off(sfield[i], O_AUTOSKIP); 
set_field_fore(sfield[i], COLOR_PAIR(2));
set_field_back(sfield[i], COLOR_PAIR(2));
}

form_driver(sfield[0],REQ_FIRST_PAGE);

my_form = new_form(sfield);
post_form(my_form);

baslik_goruntule();	

attron(COLOR_PAIR(2));
mvprintw(1, 0, _("                                 Edit Form Information                          ") );
attroff(COLOR_PAIR(2));	

mvprintw(4, 1, _("Firm Name:") );
mvprintw(6, 1, _("Adress:") );
mvprintw(8, 1, _("Tax Office:") );
mvprintw(10, 1, _("Tax Number:") );
mvprintw(12, 1, _("Telephone1:") );
mvprintw(14, 1, _("Telephone2:") );
mvprintw(16, 1, _("Fax:") );

mvprintw(LINES - 3, 0, _("                        F2->Save         ESC->Exit ") );

sprintf(query,"select sirket_adi, adres, vergi_dairesi, vergi_no, telefon1, telefon2, fax  from sirket where row_no=0 ");

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

set_field_buffer(sfield[0], 0, pgsql_kayit_field() );
set_field_buffer(sfield[1], 0, pgsql_kayit_field() );
set_field_buffer(sfield[2], 0, pgsql_kayit_field() );
set_field_buffer(sfield[3], 0, pgsql_kayit_field() );
set_field_buffer(sfield[4], 0, pgsql_kayit_field() );
set_field_buffer(sfield[5], 0, pgsql_kayit_field() );		
set_field_buffer(sfield[6], 0, pgsql_kayit_field() );		
	
touchwin(sirket_win);
wrefresh(sirket_win);
		
while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
		case KEY_F(10):
			ch=27;
			break;	
		case KEY_F (1):
		  tus_yardim();
		  form_driver (my_form, REQ_NEXT_FIELD);
		  form_driver (my_form, REQ_END_LINE);
		  refresh();
		  break;
		case 8: /*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE:
			  form_driver (my_form, REQ_PREV_CHAR);
			  form_driver (my_form, REQ_DEL_CHAR);
			  break;	
		case 9: /*TAB tusuna basýlmasý durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tuþuna basýlmasý durumunda sonraki feild gececek*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case KEY_DC:	/*del tusu*/		
    			form_driver(my_form,REQ_DEL_CHAR);
			break;			
		case KEY_HOME:
			form_driver(my_form,REQ_BEG_LINE);
			break;						
		case KEY_END:
			form_driver(my_form,REQ_END_LINE);
			break;
		case KEY_DOWN:
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;
		case KEY_UP:
			form_driver(my_form, REQ_PREV_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;
		case KEY_LEFT:
    		form_driver(my_form,REQ_LEFT_CHAR);
			break;
		case KEY_RIGHT:
    		form_driver(my_form,REQ_RIGHT_CHAR);
			break;
			
		case KEY_F(2):

/*Kayýt yapmadan evvel snaroki alana gecilmeli okuma yapmýyor aksi halde*/
form_driver(my_form, REQ_NEXT_FIELD);

sprintf(query, "delete from sirket");

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
sprintf(query, "insert into sirket (row_no, sirket_adi, adres, vergi_dairesi, vergi_no, telefon1, telefon2, fax) values (0, '%s' ,'%s' ,'%s' ,'%s' ,'%s' ,'%s', '%s')",
g_strstrip(field_buffer(sfield[0],0)),
g_strstrip(field_buffer(sfield[1],0)),
g_strstrip(field_buffer(sfield[2],0)),
g_strstrip(field_buffer(sfield[3],0)),
g_strstrip(field_buffer(sfield[4],0)),
g_strstrip(field_buffer(sfield[5],0)),
g_strstrip(field_buffer(sfield[6],0))
);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

mesaj(duzenleme_basarili);
return;
break;

		/*f2 kayýt iþlemlerinin sonu*/
		default:
			form_driver(my_form, ch);
			break;
	}
}
for (i=0;i<7;i++);free_field(sfield[i]);

delwin(sirket_win);	
}


/*****************************************************************/

int kullanici_haklari (unsigned long int kayit_no)
{
	
  char query[2000] = "";
  char gecici_yazi[1000] = "";
  int ch, n_choices, c;
  char *deger[20][60];
  char *deger2[20][60];
  char donen[60];
  int *donen_deger;
  char *yazi;

  ITEM *my_items[20];
  ITEM *cur_item;
  MENU *my_menu;
  WINDOW *kullanici_haklari_win;
    
row_no_buyuk("kullanicilar");
if (row_no<kayit_no) kayit_no=kayit_no-15;

/*burada sql den istenen tablo ve field alaný okunacak ve deðiþkenler düzenlenecek*/
sprintf (query,
"select row_no, kullanici from kullanicilar order by row_no limit 15 offset %d",
kayit_no);

if ( strlen( rtrim(giris," ")) >2) sprintf (query,
"select row_no, kullanici from kullanicilar where kullanici like '%%%s%%' order by kullanici limit 15 offset 0",
giris );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

  i = 0;
	for (j=0;j<pgsql_row_miktar;j++)  
    {
      sprintf (deger[i], "%-15s", pgsql_kayit_field() );
      sprintf (deger2[i], "%-15s", pgsql_kayit_field() );
      my_items[i] = new_item (&deger[i], &deger2[i]);
      i++;
    }

  my_items[i] = (ITEM *) NULL;
  my_menu = new_menu ((ITEM **) my_items);

  /*3 satýr 3 sutun 15 satýr 60 sutun pencere */
  kullanici_haklari_win = newwin (21, 75, 1, 3);

  set_menu_win (my_menu, kullanici_haklari_win);
  set_menu_sub (my_menu, derwin (kullanici_haklari_win, 15, 70, 4, 1));
  set_menu_format (my_menu, 15, 1);
  set_menu_mark (my_menu, mark);
  box (kullanici_haklari_win, 0, 0);

  print_in_middle (kullanici_haklari_win, 1, 0, 75, _("   F3-> New       F5-> Search    F8-> Delete    ESC-> Exit   "), COLOR_PAIR (1) );
  print_in_middle (kullanici_haklari_win, 20, 0, 75, _("  PAGE UP -> Prior Page       PAGE DOWN -> Next Page     "), COLOR_PAIR (1));

  mvwaddch (kullanici_haklari_win, 2, 0, ACS_LTEE);
  mvwhline (kullanici_haklari_win, 2, 1, ACS_HLINE, 80);
  mvwaddch (kullanici_haklari_win, 2, 79, ACS_RTEE);

  post_menu (my_menu);
  wrefresh (kullanici_haklari_win);

/*burasý çalýþmaz ise hata veriyor ve deðeri okuyamýyor*/
menu_driver (my_menu, REQ_DOWN_ITEM);
cur_item = current_item (my_menu);

refresh();

while ( (ch!= 27)  )
{      
ch=getch();
	switch(ch)
	{	
	case KEY_F(10):
		ch=27;
		break;	

	case KEY_DOWN:
	  	menu_driver (my_menu, REQ_DOWN_ITEM);
	  	break;
	
	case KEY_UP:
	  	menu_driver (my_menu, REQ_UP_ITEM);
	  	break;
	
	case KEY_F (3):	/* F3 kullanici yeni ekraný */
		kullanici_haklari_yeni();
		kullanici_haklari(0);	
		return 0;		  
	  break;
	/*
	case KEY_F (4):	// F4 kullanici duzenleme ekraný
		sprintf (gecici_yazi, "%s", item_name (cur_item));
	  	//kullanici_haklari_duzenle (gecici_yazi);
		return 0;
	*/
	case KEY_F (5):	/*arama tusu basýlýrsa */
	
		aranacak_kelime( _("Kullanýcý adýný veya yakýn bir deðer giriniz. Düzgün   sýralama için boþ býrakýnýz. ") );
		kullanici_haklari (0);
		return 0;		
	
	case KEY_F (8):	/*sil tusu basýlýrsa f8 */
		
	  donen_deger = onayla (kayit_silme_onayla);
	  if (donen_deger == 1)
	    {
	      /*Silme iþlemi onaylandý ilgili olan row_no bulunacak ve kayýt silinecek daha
	         sonra menu refresh edilmelidir */
	      	sprintf (query, "delete from kullanicilar where row_no=%s", item_name (cur_item));
	      
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		for (c = 0; c < i; ++c) free_item (my_items[c]);
	  	kullanici_haklari (kayit_no);
		refresh();
	    }
	  break;
	/*page up page down yazýlacak*/

	case KEY_NPAGE:	/* Page down basýlýrsa */

	  kayit_no = kayit_no + 15;
	
	  kullanici_haklari (kayit_no);
	  return 0;
	  break;

	case KEY_PPAGE:	/* Page up basýlýrsa */
	  if (kayit_no > 14)
	    {
	      kayit_no = kayit_no - 15;
	    }

		kullanici_haklari (kayit_no);

	  return 0;

	  break;
	}
      cur_item = current_item (my_menu);
      wrefresh (kullanici_haklari_win);
    }

  
}

	
/*****************************************************************/


int kullanici_haklari_yeni()
{
	
FIELD *field[8];
FORM  *my_form;
char deger[50];
char satir[50];
char dizi[50];
char yazi[50];
int ch,i;
char hata;
char haklar[60];
	
touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 4, 20, 0, 0);
field[1] = new_field(1, 10, 6, 20, 0, 0);
field[2] = new_field(1, 10, 8, 20, 0, 0);
field[3] = new_field(1, 1, 12, 40, 0, 0);
field[4] = new_field(1, 1, 13, 40, 0, 0);
field[5] = new_field(1, 1, 14, 40, 0, 0);
field[6] = new_field(1, 1, 15, 40, 0, 0);
field[7] = new_field(1, 1, 16, 40, 0, 0);
field[8] = NULL;

for (i=0;i<8;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                          New User Adding Screen                                ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(4, 1, _("User Name        :") );
mvprintw(6, 1, _("Password         :") );
mvprintw(8, 1, _("Password (again) :") );

mvprintw(10, 25, _("***        Rights      ***") );

mvprintw(12, 1, _("Stock          :") );
mvprintw(13, 1, _("Currency       :") );
mvprintw(14, 1, _("Reports        :") );
mvprintw(15, 1, _("Delete Record  :") );
mvprintw(16, 1, _("New Record     :") );

//koseli parantezler koyulacak

mvprintw(12, 39, "[ ]");
mvprintw(13, 39, "[ ]");
mvprintw(14, 39, "[ ]");
mvprintw(15, 39, "[ ]");
mvprintw(16, 39, "[ ]");

mvprintw(LINES - 3, 0, _("      F2 -> Save                                   ESC-> Exit               ") );

mvprintw(LINES - 7, 0, _(" - Boþluk tuþu ile seçebilirsiniz, del tuþu ile silinebilir") );
mvprintw(LINES - 6, 0, _(" - Kullanýcý adýna hangi isimle girecek ise o yazýlýr.   ") );
mvprintw(LINES - 5, 0, _(" - Þifre her iki seferde düzgün olarak yazýlmalýdýr.	") );
	
refresh();

		
while ( (ch!= 27)  )
{      
ch=getch();
	switch(ch)
	{	
		case KEY_F(10):
			ch=27;
			break;

		case KEY_F (1):
		  	tus_yardim();
		  	form_driver (my_form, REQ_NEXT_FIELD);
		  	form_driver (my_form, REQ_END_LINE);
		  	refresh();
		  	break;
		case 8: /*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr*/
    			form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE:
			form_driver (my_form, REQ_PREV_CHAR);
			form_driver (my_form, REQ_DEL_CHAR);
			break;	
		case 9: /*TAB tusuna basýlmasý durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tuþuna basýlmasý durumunda sonraki feild gececek*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case KEY_DC:	/*del tusu*/		
	    		form_driver(my_form,REQ_DEL_CHAR);
			break;			
		case KEY_HOME:
			form_driver(my_form,REQ_BEG_LINE);
			break;		
		case KEY_END:
			form_driver(my_form,REQ_END_LINE);
			break;
		case KEY_DOWN:
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;
		case KEY_UP:
			form_driver(my_form, REQ_PREV_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;
		case KEY_LEFT:
    			form_driver(my_form, REQ_LEFT_CHAR);
			break;
		case KEY_RIGHT:
    			form_driver(my_form, REQ_RIGHT_CHAR);
			break;
		case  KEY_F(2): /*kaydet*/
			form_driver(my_form, REQ_NEXT_FIELD);
		
			if (!strstr( rtrim(field_buffer(field[1],0),""), rtrim(field_buffer(field[2],0),"") ))
			{mesaj( _("Passwords is not same!") ); break; }
		
			strcpy(haklar,"");
				
			for (i=3;i<8;i++)
			{
			sprintf(deger,"%s", rtrim(field_buffer(field[i],0),"")  );
			if (strstr(deger,"X")) strcat(haklar,"1"); else strcat(haklar,"0");
		    }
				
	    	/*rowno alaný alýyor */
		  row_no_buyuk("kullanicilar");
			
		  sprintf (query,
		   "insert into kullanicilar (row_no, kullanici, sifre, haklar) values (%d, '%s','%s','%s')",
		   /*row_no, kullanici */
		   row_no+1, g_strstrip(field_buffer (field[0], 0)),
		   /*sifre, haklar */
		   sifre_kripto_yap(g_strstrip(field_buffer (field[1], 0) ) ) , haklar
		   );

		  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
			mesaj( _("New user succesfully created.") );
			return;

		case 32: //space tusu 32 kodunu içermektedir.
			//set_field_buffer(field[0], 0,  "");
			//set_field_buffer(field[1], 0,  "");
			//set_field_buffer(field[2], 0,  "");
			//X 88 denk gelir
			form_driver(my_form, 88);		
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}


}
	
	
	
//bitis	
}

/*****************************************************************/

int kullanici_haklari_duzenle(char kullanici[])
{
	mesaj( _("Edit screen not ready for use.") );
}

/*****************************************************************/



int yazici_ayarlari()
{
char deger[50];
char yazi[50];
int ch,i;
	
touchwin(ana_win);
wrefresh(ana_win);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                           Printers Information Screen                          ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(LINES - 3, 0, _("F2 -> Save     F5-> Print Test Page       F8-> Add New Printer      ESC-> Exit ") );

mvprintw(6, 0, _(" - Acikiþletme sisteme kurulmuþ olan varsayýlan yazýcýyý kullanýr.") );
mvprintw(8, 0, _(" - Bu amaçla sisteme bir yazýcý kurmalýsýnýz. Eðer kurmadýysanýz") ); 
mvprintw(10, 0, _(" - cups tavsiye edilir. F8 tuþuna basarak CUPS yöneticisini") );
mvprintw(12, 0, _(" - kullanabilirsiniz.  Eðer sisteminizde yazýcý kuruluysa F5 ") );
mvprintw(14, 0, _(" - tuþuna basarak deneme yapabilirsiniz. Acikiþletme içinde   ") );
mvprintw(16, 0, _(" - yazýcý çýktýlarýný almak için sisteminizde enscript kurulu olmalýdýr.") ); 

	refresh();
	
	while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
		case KEY_F(10):
			ch=27;
			break;	
		case KEY_F (1):
		  	tus_yardim();
		  	refresh();
		  break;
		case  KEY_F(5): /*test çýktýsý*/							
			test_sayfasi_yazdir();
			mesaj( _("Test page sended to printer.") );
			wrefresh(ana_win);			
			break;				
		case  KEY_F(8): /*sisteme yazýcý ekle*/
			system("lynx localhost:631");			
			wrefresh(ana_win);
			return;
			break;		
		default:
			break;
				
		}

}

}


/*****************************************************************/

int test_sayfasi_yazdir()
{	
FILE *dosya;
char oku[250];

//test page must be multi language
system("enscript --quiet --encoding=88599 --font=Tahoma12 /opt/acikisletme/conf/test.txt -B -MA4");
//system("rm -rf /opt/acikisletme/conf/stoklistesi");
}


/*****************************************************************/	

int yedekleme_ayarlari()
{
FIELD *yfield[2];
FORM  *my_formy;
FILE *dosya;
char deger[50];
char satir[50];
char dizi[50];
char yazi[50];
int ch,i;
char hata;

char otoyedek[40];
char yedektar[40];
	
touchwin(ana_win);
wrefresh(ana_win);

yfield[0] = new_field(1, 1, 4, 45, 0, 0);
yfield[1] = new_field(1, 15, 8, 45, 0, 0);
yfield[2] = NULL;

field_opts_off(yfield[0], O_AUTOSKIP); 
field_opts_off(yfield[1], O_AUTOSKIP); 

form_driver(yfield[0],REQ_FIRST_PAGE);

my_formy = new_form(yfield);
post_form(my_formy);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                            Backup Configuration                                ") );
attroff(COLOR_PAIR(1));	

mvprintw(4, 1, _("Backup in everday first run        : ") );
mvprintw(8, 1, _("Last backup date                   : ") );

//koseli parantezler koyulacak
mvprintw(4, 44, "[ ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("                 F2 -> Save                    ESC-> Exit ") );
	
mvprintw(LINES - 12, 0, _(" - Uygulama hergün ilk açýlýþta yedek almasýnýn istenmesi durumunda; ") );
mvprintw(LINES - 11, 0, _("   seçim kutusunun aktif durumda olmalýdýr. ") );
mvprintw(LINES - 10, 0, _(" - Uygulama bütün yedekleri /opt/acikisletme/backup dizini içine alýr.") );
mvprintw(LINES - 9, 0, _(" - Bu dizin içerisinde geri yüklenmek istenen tarih kullanýlarak aþaðýdaki ") );
mvprintw(LINES - 8, 0, _("   satýrlarýn yazýlmasý yeterlidir.") );
mvprintw(LINES - 7, 0, _("   su postgres ") );
mvprintw(LINES - 6, 0, _("   createdb acikisletme") );
mvprintw(LINES - 5, 0, _("   psql acikisletme < istenen_tarih.sql") );


	dosya=fopen("/opt/acikisletme/conf/yedek.conf","r");
	while (!feof(dosya))
		{	
	fgets(satir, 50,  dosya);
		if (strcmp(satir,"#" )!=-1)
			{		
			//hergun otoyedek alsýnmý
			if (strstr(satir,"otoyedek")) {strcpy(dizi,&satir[7]);rtrim(dizi,"\n"); rtrim(dizi," ");strcpy(otoyedek,dizi);  }
			//en son yedekleme tarihi
			if (strstr(satir,"yedektar")) { strcpy(dizi,&satir[9]); rtrim(dizi,"\n"); rtrim(dizi," "); strcpy(yedektar, dizi); }
			}
		}
	fclose(dosya);		

/*okunan degerler field içlerine yerleþtirelecek*/
		
	if (strstr(otoyedek,"true")) 	set_field_buffer(yfield[0], 0,  "X");
	set_field_buffer(yfield[1], 0,  yedektar);
	refresh();
	
	while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
		case KEY_F(10):
			ch=27;
			break;	
		case KEY_F (1):
		  	tus_yardim();
		  	form_driver (my_formy, REQ_NEXT_FIELD);
		  	form_driver (my_formy, REQ_END_LINE);
		  	refresh();
		  	break;
		case 8: /*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr*/
    			form_driver(my_formy, REQ_PREV_CHAR);
			form_driver(my_formy, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE:
			form_driver (my_formy, REQ_PREV_CHAR);
			form_driver (my_formy, REQ_DEL_CHAR);
			break;	
		case 9: /*TAB tusuna basýlmasý durumunda sonraki field konumlan*/
			form_driver(my_formy, REQ_NEXT_FIELD);
			form_driver(my_formy, REQ_END_LINE);
			break;		
		case 10: /*ENTER tuþuna basýlmasý durumunda sonraki feild gececek*/
			form_driver(my_formy, REQ_NEXT_FIELD);
			form_driver(my_formy, REQ_END_LINE);
			break;		
		case KEY_DC:	/*del tusu*/		
    			form_driver(my_formy,REQ_DEL_CHAR);
			break;			
		case KEY_HOME:
			form_driver(my_formy,REQ_BEG_LINE);
			break;		
		case KEY_END:
			form_driver(my_formy,REQ_END_LINE);
			break;
		case KEY_DOWN:
			form_driver(my_formy, REQ_NEXT_FIELD);
			form_driver(my_formy, REQ_END_LINE);
			break;
		case KEY_UP:
			form_driver(my_formy, REQ_PREV_FIELD);
			form_driver(my_formy, REQ_END_LINE);
			break;
		case KEY_LEFT:
    			form_driver(my_formy,REQ_LEFT_CHAR);
			break;
		case KEY_RIGHT:
    			form_driver(my_formy,REQ_RIGHT_CHAR);
			break;
		case  KEY_F(2): /*kaydet*/
			form_driver(my_formy, REQ_NEXT_FIELD);
		
			sprintf(deger,"%s", rtrim(field_buffer(yfield[0],0),"")  );			
			if (strstr(deger,"X")) strcpy(yazi,"true"); else strcpy(yazi,"false");
						
			dosya=fopen("/opt/acikisletme/conf/yedek.conf","w");
			
			//otoyedek degiskeni yazýldý true false
			sprintf(deger,"otoyedek=%s\n", yazi );
			fputs(deger,dosya);
			
			//yedektar icine en son yedek alýnan tarih yazýldý
			sprintf(deger,"yedektar=%s\n", rtrim(field_buffer(yfield[1],0),"")  );
			fputs(deger,dosya);
			
			fclose(dosya);
			mesaj( _("Record Succesfully Saved.") );
			ch=NULL;
			form_driver(my_formy, REQ_NEXT_FIELD);
			form_driver(my_formy, REQ_PREV_FIELD);
			break;		

		case 32: //space tusu 32 kodunu içermektedir.
		
			if (strstr(field_buffer(yfield[0],0),"X") )	set_field_buffer(yfield[0], 0,  "");
				else set_field_buffer(yfield[0], 0,  "X");
			//form_driver(my_form, 88);		
			break;
		
		default:
			form_driver(my_formy, ch);
			break;
				
		}


}

for (i=0;i<2;i++);free_field(yfield[i]);

}

/*****************************************************************/

int kullanici_onayla_ayarlar()
{
	FIELD *field[3];
	FORM  *my_form;
	int ch;
	WINDOW *kullanici_onayla_win;
	
	touchwin(ana_win);
	wrefresh(ana_win);
			
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
		
	kullanici_onayla_win=newwin(0,0,24,79);
	touchwin(kullanici_onayla_win);
	wrefresh(kullanici_onayla_win);
		
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);

	/*calýþma yýlý 4 karakter*/
	field[0] = new_field(1, 10, 18, 42, 0, 0);
	/*firma 2 karakter*/
	field[1] = new_field(1, 10, 20, 42, 0, 0);
	field[2] = NULL;

	set_field_fore(field[0], COLOR_PAIR(1));
	set_field_fore(field[1], COLOR_PAIR(1));
	/*geri plan rengi olsun*/
	set_field_back(field[0], COLOR_PAIR(2));
	set_field_back(field[1], COLOR_PAIR(2));
	/*
	set_field_type(field[0], TYPE_INTEGER);
	set_field_type(field[1], TYPE_INTEGER);
	*/
	field_opts_off(field[0], O_AUTOSKIP); 
	field_opts_off(field[1], O_AUTOSKIP); 

	/*sifre alaný olan ikinci field password biçiminde olmalý*/
	field_opts_off(field[1], O_PUBLIC);

	/*geri plan rengi olmasýn*/
	/*set_field_back(field[0], A_UNDERLINE); 
	set_field_back(field[1], A_UNDERLINE); 
	*/
	my_form = new_form(field);
	post_form(my_form);
	refresh();
	
	set_current_field(my_form, field[0]); 

	attroff(1);
	attroff(2);
	
	mvprintw(18, 25, _("User Name     :") );
	mvprintw(20, 25, _("Password      :") );
	
	mvprintw(3, 20, _("Kullanýcý ekraný giriþ sorgulamasý") );
	mvprintw(2, 10, _("Bu alana giriþ için root kullanýcýsýnýn haklarý gerekmektedir.") );
	mvprintw(LINES-3, 5, _("F2-> Okey                                                ESC-> Exit ") );

	
mvprintw(6,3, _("                             Open                                    ") );
mvprintw(7,3,"  _|              _|                _|                                  ");
mvprintw(8,3,"         _|_|_|   _|     _|_|     _|_|_|_|   _|_|_|  _|_|       _|_|    ");
mvprintw(9,3,"  _|   _|_|       _|   _|_|_|_|     _|       _|    _|    _|   _|_|_|_|  ");
mvprintw(10,3,"  _|       _|_|   _|   _|           _|       _|    _|    _|   _|        ");
mvprintw(11,3,"  _|   _|_|_|     _|     _|_|_|       _|_|   _|    _|    _|     _|_|_|  ");
mvprintw(13,3, _("               Açýk iþletme GPL bir ticari uygulamadýr.") );

refresh();
			
	while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
		case KEY_F(10):
			ch=27;
			break;	
		case KEY_F (1):
		  	tus_yardim();
		  	form_driver (my_form, REQ_NEXT_FIELD);
		  	form_driver (my_form, REQ_END_LINE);
		  	refresh();
		  	break;
		case 8: /*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr konsolda 8 xterm key_backspace*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE:
			form_driver (my_form, REQ_PREV_CHAR);
			form_driver (my_form, REQ_DEL_CHAR);
			break;	
		case 9: /*TAB tusuna basýlmasý durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tuþuna basýlmasý durumunda sonraki feild gececek*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case KEY_DC:	/*del tusu*/		
    			form_driver(my_form,REQ_DEL_CHAR);
			break;			
		case KEY_HOME:
			form_driver(my_form,REQ_BEG_LINE);
			break;						
		case KEY_END:
			form_driver(my_form,REQ_END_LINE);
			break;
		case KEY_DOWN:
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;
		case KEY_UP:
			form_driver(my_form, REQ_PREV_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;
		case KEY_LEFT:
    			form_driver(my_form,REQ_LEFT_CHAR);
			break;
		case KEY_RIGHT:
    			form_driver(my_form,REQ_RIGHT_CHAR);
			break;
		case 27: /*esc cýkýs basýlmasý durumunda*/
			mvprintw(3, 20, "                                  ");
			mvprintw(2, 10, "                                                              ");
			refresh();
			return ;
			break;		
		case KEY_F(2): 	/*f2 giris basýlmasý durumunda*/
			form_driver(my_form, REQ_PREV_FIELD);
			strcpy(query, "select sifre from kullanicilar where kullanici='root'");
			if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			sprintf(sifre,"%s", pgsql_kayit_field() );
			if (( strcmp(g_strstrip(field_buffer(field[0],0)), "root") ==NULL ) &
						(sifre_kripto_coz( g_strstrip(field_buffer(field[1],0)) , sifre) == 0)  )   
							{
							//kullanýcý root ise haklar full olmalý 							
							sprintf(haklar,"%s", "1111111111111111111111111111111111111111111111111111111111111111");
							beep();
							kullanici_haklari(0);
															
							free_field(field[0]);
							free_field(field[1]);
							touchwin(ana_win);
							wrefresh(ana_win);															
							return;								
							}
							else
							{							
							set_field_buffer(field[0],0," ");
							set_field_buffer(field[1],0," ");
							refresh();
							//beep sesi koyulacak
							beep();
							}
		
				break;
			
			default:
				form_driver(my_form, ch);
				break;

		}
	}
	
delwin(kullanici_onayla_win);	

/*son*/
}

/*****************************************************************/





