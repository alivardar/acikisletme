
int raporlar()
{
ITEM **my_items;
int c;				
MENU *my_menu;
WINDOW *my_menu_win;
int n_choices, i;
ITEM *cur_item;	

char *choices[] = {
        _("Account List"),
	_("Adress List"),
	_("Existing Stock List"),
	_("Waybill Without Bill"),
	_("Debt/Credit List"),
	_("Checks in/out List"),
	_("Back"),
        (char *)NULL,
                  };
	
	baslik_goruntule();
				  
	touchwin(ana_win);
	wrefresh(ana_win);

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
	print_in_middle(my_menu_win, 1, 0, 45, "Reports", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 48);
	mvwaddch(my_menu_win, 2, 49, ACS_RTEE);
        
	post_menu(my_menu);
	wrefresh(my_menu_win);
	
	while ( (c!= 27)  )
	{      
	c=getch();
		switch(c)
		{	
			case KEY_F(10):
				c=27;
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
					case 1:	
						cari_hesap_listesi_raporu();
					
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
					case 2:							
						adres_etiket_raporu();
					
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
					
						break;					
					case 3:	/*Stok Listesi Mevcut d�k�m�*/
						stok_listesi_raporu();
						
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
					case 4:	
						faturasiz_irsaliyeler_raporu();
					
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
					
						break;
					case 5:	
						borc_alacak_raporu();
					
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
					
					case 6:	
						//cek hareketleri raporu;
						cek_hareket_raporu();
					
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
					
						break;
										
					case 7:	/*Ana Men�ye D�n��*/
						
						unpost_menu(my_menu);
				        free_menu(my_menu);
				        for(i = 0; i < n_choices; ++i)  free_item(my_items[i]);
						return 0;
				        }
						
						break;

				refresh();
				pos_menu_cursor(my_menu);
		}
                wrefresh(my_menu_win);
	}	


	return 0;
}



/**********************************************************/


int cari_hesap_listesi_raporu()
{
FIELD *field[3];
FORM  *my_form;
WINDOW *my_menu_win;
int ch,i;
char hata;
int basno;
int bitno;
char *deger[20][60];
	
touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 6, 25, 0, 0);
field[1] = new_field(1, 10, 6, 65, 0, 0);
field[2] = new_field(1, 1, 10, 20, 0, 0);
field[3] = new_field(1, 1, 12, 20, 0, 0);
field[4] = NULL;

for (i=0;i<5;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, "                           Account List Report                                  ");
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(6, 1, _("First no.:") );
mvprintw(6, 40, _("Last no.   :") );

mvprintw(8, 1, _("Please select out device.") );
mvprintw(10, 1, _("Screen   : ") );
mvprintw(12, 1, _("Printer  : ") );

//koseli parantezler koyulacak

mvprintw(6, 24, "[          ]");
mvprintw(6, 64, "[          ]");
mvprintw(10, 19, "[ ]");
mvprintw(12, 19, "[ ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("      F5 -> Report                                 ESC-> Exit           ") );
	
mvprintw(LINES - 7, 0, _(" - �stenen ba�lang�� ve biti� kay�t numaralar� yaz�lmal�d�r.") );
mvprintw(LINES - 6, 0, _(" - Rapor ��kt�s� ayg�t se�imi bo�luk tu�una basarak yap�n�z.") );

	
	/*ba�lang�� ve biti de�erleri �ncednen ilk ve son kay�t yaz�lacak*/
	set_field_buffer(field[0], 0,  "1");
	row_no_buyuk("firmalar");
	set_field_buffer(field[1], 0,  rowno);
	
	set_field_buffer(field[2], 0,  "X");
	set_field_buffer(field[3], 0,  "");
		
	refresh();

	
	while ( (ch!= 27)  )
	{      
	ch=getch();
		switch(ch)
		{	
		case KEY_F(10):
			ch=27;
			break;
		case 8: /*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case KEY_BACKSPACE:
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		
		case 9: /*TAB tusuna bas�lmas� durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tu�una bas�lmas� durumunda sonraki feild gececek*/
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
		case  KEY_F(5): /*rapor ��kt�s�*/
						
			form_driver(my_form, REQ_NEXT_FIELD);		
			basno=atoi( rtrim(field_buffer(field[0],0)," ") );			
			bitno=atoi( rtrim(field_buffer(field[1],0)," ") );			
			sprintf(deger,"%s", rtrim(field_buffer(field[2],0),"")  );
		
			if (strstr(deger,"X")) cari_hesap_listesi_raporu_yazdir( 0, basno, bitno);
			
			sprintf(deger,"%s", rtrim(field_buffer(field[3],0),"")  );
			if (strstr(deger,"X")) cari_hesap_listesi_raporu_yazdir( 1, basno, bitno);

			refresh();
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_PREV_FIELD);
			
			break;						
		case 32: //space tusu 32 kodunu i�ermektedir.
			/*ekran yaz�c� bolumu s�f�rlanmal�d�r*/
			set_field_buffer(field[2], 0,  "");
			set_field_buffer(field[3], 0,  "");
			form_driver(my_form, 88);		//88 b�y�k X tu�una kar��l�k gelmektedir.
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}


}

for (i = 0; i < 4; i++);  free_field (field[i]);			
}


/************************************************************************/

int cari_hesap_listesi_raporu_yazdir(int cikti,int basno, int bitno)
{

char *deger[20][60];
int i,j;
WINDOW *rapor_pencere;
char yazi[78];
	
char baslik_yazisi[250];
char sirket_adi[100];
	
sprintf(query, "select sirket_adi from sirket");

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
sprintf(sirket_adi,"%s", pgsql_kayit_field() );
	
if (cikti==0)//0 ise ekran
{

rapor_pencere = newwin (24, 80, 0, 0);
keypad (rapor_pencere, TRUE);
box (rapor_pencere, 0, 0);
print_in_middle (rapor_pencere, 1, 30, 22, _("Account List Report"), COLOR_PAIR (1));
wrefresh(rapor_pencere);
sprintf(query, "select bilanco_no, firma_adi, telefon, adres from firmalar where row_no>%d and row_no<%d order by row_no",
		basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

i=2;
for (j=0;j<pgsql_row_miktar;j++)
{	
	sprintf(yazi,"%6.6s|", pgsql_kayit_field());
	sprintf(yazi,"%s%25.25s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%15.15s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%25.25s|", yazi, pgsql_kayit_field() );
		
	mvwprintw (rapor_pencere,i, 1, "%.78s", yazi);

i++;	
wrefresh(rapor_pencere);
if (i>23)	{i=2;while (1){ if (getch()==32); break;} }
	
}
while (1){ if (getch()==32); break;}
delwin(rapor_pencere);
refresh();
}

else
{
	//yaziciya gonder secili ise

	yazici=fopen("/opt/acikisletme/carihesaplistesi","w");
	
//ilk olarak sayfan�n en ba��na uygulama ad� s�r�m� ve sol k��eyede tarih saat atal�m
	tarih_ayarla();
	sprintf(baslik_yazisi,
	_("%s Firm Account List    ---     Report time : %s/%s/%s   %s:%s:%s  ---   User:%s\n\n"),
	sirket_adi, gun, ay, yil, saat, dakika, saniye, kullanici);
	fputs(baslik_yazisi, yazici);
	fputc(13, yazici);
	
	//font k�sm�  baslik buyuk olacak 15 punto
	fputs(_("                                                            Account List Report\n"),yazici);
	fputs("__________________________________________________________________________________________________________________________________________________\n",yazici);
	
	//konu ba�l�klar� at�l�yor
	sprintf(deger,
"|%5.5s|%6.6s|%-20.20s|%-30.30s|%-15.15s|%15.15s|%15.15s|%15.15s|%15.15s|\n",
_("No.") , _(" F.No."), _("Firma Ad�"), _("Adres"), _("Vergi Dairesi"), _("Vergi No."), _("Telefon1"), _("Fax"), _("A��klama") );
	fputs(deger, yazici);

fputs("__________________________________________________________________________________________________________________________________________________\n",yazici);

sprintf(query, "select * from firmalar where row_no>%d and row_no<%d order by row_no",
	basno-1, bitno+1);
	
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
i=0;
for (j=0;j<pgsql_row_miktar;j++)
    {	
sprintf(alan1, "%s", pgsql_kayit_field());
sprintf(alan2, "%s", pgsql_kayit_field());
sprintf(alan3, "%s", pgsql_kayit_field());
sprintf(alan4, "%s", pgsql_kayit_field());
sprintf(alan5, "%s", pgsql_kayit_field());
sprintf(alan6, "%s", pgsql_kayit_field());
sprintf(alan7, "%s", pgsql_kayit_field());
sprintf(alan8, "%s", pgsql_kayit_field());
sprintf(alan9, "%s", pgsql_kayit_field());

sprintf(deger,
"|%5.5s|%6.6s|%-20.20s|%-30.30s|%-15.15s|%15.15s|%15.15s|%15.15s|%15.15s|\n",
	alan1, alan2, alan3, alan4, alan5, alan6, alan7, alan8, alan9 );
		
	fputs(deger, yazici);
	i++;
	//if (i>40) fputc(12, yazici);
	
	}
	
	fclose( yazici );
	
	system("enscript --quiet --encoding=88599 --font=Courier8 /opt/acikisletme/carihesaplistesi -B -r -MA4dj");
	system("rm -rf /opt/acikisletme/carihesaplistesi");
	
	mesaj( _("Report sended to printer.") );
		
}
	

//bitis	
}



/************************************************************/

int stok_listesi_raporu()
{

FIELD *field[3];
FORM  *my_form;
char deger[50];
int ch,i;
int basno;
int bitno;
	
touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 6, 25, 0, 0);
field[1] = new_field(1, 10, 6, 65, 0, 0);
field[2] = new_field(1, 1, 10, 20, 0, 0);
field[3] = new_field(1, 1, 12, 20, 0, 0);
field[4] = NULL;

for (i=0;i<5;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                                Stok Listesi Raporu                            ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(6, 1, _("Ba�lang�� kay�t no :") );
mvprintw(6, 40, _("Biti� kay�t no    :") );

mvprintw(8, 1, _("��kt� i�in kullan�lacak olan ayg�t� se�iniz.") );
mvprintw(10, 1, _("Ekran   : ") );
mvprintw(12, 1, _("Yaz�c�  : ") );

//koseli parantezler koyulacak

mvprintw(6, 24, "[          ]");
mvprintw(6, 64, "[          ]");
mvprintw(10, 19, "[ ]");
mvprintw(12, 19, "[ ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("      F5 -> Rapor                                 ESC-> ��k��           ") );
	
mvprintw(LINES - 7, 0, _(" - �stenen ba�lang�� ve biti� kay�t numaralar� yaz�lmal�d�r.") );
mvprintw(LINES - 6, 0, _(" - Rapor ��kt�s� ayg�t se�imi bo�luk tu�una basarak yap�n�z.") );


	/*ba�lang�� ve biti de�erleri �ncednen ilk ve son kay�t yaz�lacak*/
	set_field_buffer(field[0], 0,  "1");
	row_no_buyuk("stokkart");
	set_field_buffer(field[1], 0,  rowno);
	
	set_field_buffer(field[2], 0,  "X");
	set_field_buffer(field[3], 0,  "");
		
	refresh();

ch=0;
while ((ch != 27))
    {
      ch = getch ();
      switch (ch)
	{
		case KEY_F (10):
			  ch = 27;
			  break;
		case 8: /*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case KEY_BACKSPACE:
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		
		case 9: /*TAB tusuna bas�lmas� durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tu�una bas�lmas� durumunda sonraki feild gececek*/
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
		case  KEY_F(5): /*rapor ��kt�s�*/
						
			form_driver(my_form, REQ_NEXT_FIELD);		
			basno=atoi( rtrim(field_buffer(field[0],0)," ") );			
			bitno=atoi( rtrim(field_buffer(field[1],0)," ") );			
			sprintf(deger,"%s", rtrim(field_buffer(field[2],0),"")  );
		
			if (strstr(deger,"X")) stok_listesi_raporu_yazdir( 0, basno, bitno);
			
			sprintf(deger,"%s", rtrim(field_buffer(field[3],0),"")  );
			if (strstr(deger,"X")) stok_listesi_raporu_yazdir( 1, basno, bitno);
									
			refresh();
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_PREV_FIELD);
			
			break;						
		case 32: //space tusu 32 kodunu i�ermektedir.
			/*ekran yaz�c� bolumu s�f�rlanmal�d�r*/
			set_field_buffer(field[2], 0,  "");
			set_field_buffer(field[3], 0,  "");
			form_driver(my_form, 88);		//88 b�y�k X tu�una kar��l�k gelmektedir.
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}


}

for (i = 0; i < 4; i++);  free_field (field[i]);	
}



/************************************************************/

int stok_listesi_raporu_yazdir(int cikti,int basno, int bitno)
{
	
char *deger[20][60];
int i,j;
WINDOW *rapor_pencere;
	
char baslik_yazisi[250];
char sirket_adi[100];
	
//Firma ad� al�nacak 
sprintf(query, "select sirket_adi from sirket");

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

sprintf(sirket_adi,"%s", pgsql_kayit_field() );
	
if (cikti==0)//0 ise ekran
{
rapor_pencere = newwin (24, 80, 0, 0);
keypad (rapor_pencere, TRUE);
box (rapor_pencere, 0, 0);
print_in_middle (rapor_pencere, 1, 30, 22, "Stok Listesi Raporu", COLOR_PAIR (1));
wrefresh(rapor_pencere);
sprintf(query, "select malzeme_kodu, malzeme_adi, seri_no, mevcut, birim_tip, kdv_orani, birim_fiyat from stokkart where row_no>%d and row_no<%d order by row_no",
		basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

i=2;
for (j=0;j<pgsql_row_miktar;j++)
{	
	sprintf(yazi,"%6.6s|", pgsql_kayit_field());
	sprintf(yazi,"%s%20.20s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%6.6s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%10.10s", yazi, yazidan_paraya_cevir(pgsql_kayit_field()) );
	sprintf(yazi,"%s%5.5s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%2.2s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%20.20s|", yazi, yazidan_paraya_cevir(pgsql_kayit_field()) );
	
	mvwprintw (rapor_pencere,i, 1, "%.78s", yazi);

i++;	
wrefresh(rapor_pencere);
if (i>23)	{i=2;while (1){ if (getch()==32); break;} }
	
}
while (1){ if (getch()==32); break;}
delwin(rapor_pencere);
refresh();
}

else
{
	//yaziciya gonder secili ise	
	yazici=fopen("/opt/acikisletme/conf/stoklistesi","w");
	
	tarih_ayarla();
	sprintf(baslik_yazisi,
	_("%s �irketi Stok Kart� D�k�m� %s/%s/%s   %s:%s:%s kullan�c�: %s\n\n"),
	sirket_adi, gun, ay, yil, saat, dakika, saniye, kullanici);
	fputs(baslik_yazisi, yazici);
		
	fputs(_("                                           Stok �r�n Listesi\n"), yazici);
	fputs("______________________________________________________________________________________________________________\n",yazici);
	sprintf(deger,
"|%4.4s|%15.15s|%-20.20s|%-10.10s|%-15.15s|%20.20s|%16.16s|\n",
_("No."), _("Malzeme Kodu"), _("�r�n Ad�"), _("A��klama"), _("Seri No."), _("Birim Fiyat"), _("Mevcut") );
	fputs(deger, yazici);
	
	fputs("______________________________________________________________________________________________________________\n",yazici);

  sprintf(query, "select * from stokkart where row_no>%d and row_no<%d order by row_no",
	basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
i=0;
  for (j=0;j<pgsql_row_miktar;j++)
    {	
sprintf(alan1, "%s", pgsql_kayit_field());
sprintf(alan2, "%s", pgsql_kayit_field());
sprintf(alan3, "%s", pgsql_kayit_field());
sprintf(alan4, "%s", pgsql_kayit_field());
sprintf(alan5, "%s", pgsql_kayit_field());
sprintf(alan6, "%s", yazidan_paraya_cevir(pgsql_kayit_field()) );
sprintf(alan7, "%s", pgsql_kayit_field());
sprintf(alan8, "%s", pgsql_kayit_field());
sprintf(alan9, "%s", pgsql_kayit_field());
	
	sprintf(deger,
	"|%4.4s|%15.15s|%-20.20s|%-10.10s|%-15.15s|%20.20s|%10.10s %5.5s|\n",
	alan1, alan2, alan3, alan4, alan5, alan6, alan7, alan9 );

	fputs(deger, yazici);
	
	i++;
//	if (i>50) fputc(12,yazici);		
	}
		
	fclose( yazici );
	
	system("enscript --quiet --encoding=88599 --font=Courier8 /opt/acikisletme/conf/stoklistesi -B -MA4");
	system("rm -rf /opt/acikisletme/conf/stoklistesi");
	
	mesaj( _("Report sended to printer.") );
	refresh();
		
}
	
//bitis	
}



/************************************************************/


int faturasiz_irsaliyeler_raporu()
{

FIELD *field[3];
FORM  *my_form;
int ch,i;
char *deger[20][60];

int basno;
int bitno;
	
touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 6, 25, 0, 0);
field[1] = new_field(1, 10, 6, 65, 0, 0);
field[2] = new_field(1, 1, 10, 20, 0, 0);
field[3] = new_field(1, 1, 12, 20, 0, 0);
field[4] = NULL;

for (i=0;i<5;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                        Faturas� Kesilmemi� �rsaliyeler Raporu                 ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(6, 1, _("Ba�lang�� kay�t no :") );
mvprintw(6, 40, _("Biti� kay�t no    :") );

mvprintw(8, 1, _("��kt� i�in kullan�lacak olan ayg�t� se�iniz.") );
mvprintw(10, 1, _("Ekran   : ") );
mvprintw(12, 1, _("Yaz�c�  : ") );

//koseli parantezler koyulacak

mvprintw(6, 24, "[          ]");
mvprintw(6, 64, "[          ]");
mvprintw(10, 19, "[ ]");
mvprintw(12, 19, "[ ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("      F5 -> Report                                 ESC-> Exit           ") );
	
mvprintw(LINES - 7, 0, _(" - �stenen ba�lang�� ve biti� kay�t numaralar� yaz�lmal�d�r.") );
mvprintw(LINES - 6, 0, _(" - Rapor ��kt�s� ayg�t se�imi bo�luk tu�una basarak yap�n�z.") );

	
	
	/*ba�lang�� ve biti de�erleri �ncednen ilk ve son kay�t yaz�lacak*/
	set_field_buffer(field[0], 0,  "1");
	row_no_buyuk("irsaliyeler");
	set_field_buffer(field[1], 0,  rowno);
	
	set_field_buffer(field[2], 0,  "X");
	set_field_buffer(field[3], 0,  "");
		
	refresh();
		
while ( (ch!= 27)  )
{      
ch=getch();
	switch(ch)
		{			
		case KEY_F(10):
			ch=27;
			break;			
		case 8: /*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case KEY_BACKSPACE:
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case 9: /*TAB tusuna bas�lmas� durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tu�una bas�lmas� durumunda sonraki feild gececek*/
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
		case  KEY_F(5): /*rapor ��kt�s�*/
						
			form_driver(my_form, REQ_NEXT_FIELD);		
			basno=atoi( rtrim(field_buffer(field[0],0)," ") );			
			bitno=atoi( rtrim(field_buffer(field[1],0)," ") );			
			sprintf(deger,"%s", rtrim(field_buffer(field[2],0),"")  );
		
			if (strstr(deger,"X")) faturasiz_irsaliyeler_raporu_yazdir( 0, basno, bitno);
			
			sprintf(deger,"%s", rtrim(field_buffer(field[3],0),"")  );
			if (strstr(deger,"X")) faturasiz_irsaliyeler_raporu_yazdir( 1, basno, bitno);
				
			refresh();
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_PREV_FIELD);
					
			break;						
		case 32: //space tusu 32 kodunu i�ermektedir.
			/*ekran yaz�c� bolumu s�f�rlanmal�d�r*/
			set_field_buffer(field[2], 0,  "");
			set_field_buffer(field[3], 0,  "");
			form_driver(my_form, 88);		//88 b�y�k X tu�una kar��l�k gelmektedir.
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}


}

for (i = 0; i < 4; i++);  free_field (field[i]);	
}



/************************************************************/

int faturasiz_irsaliyeler_raporu_yazdir(int cikti,int basno, int bitno)
{
	
char *deger[20][60];
int i,j;
WINDOW *rapor_pencere;
char yazi[75];
	
char baslik_yazisi[250];
char sirket_adi[100];
	
//Firma ad� al�nacak 
sprintf(query, "select sirket_adi from sirket");
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	
sprintf(sirket_adi,"%s", pgsql_kayit_field() );
	
if (cikti==0)//0 ise ekran
{

rapor_pencere = newwin (24, 80, 0, 0);
keypad (rapor_pencere, TRUE);
box (rapor_pencere, 0, 0);
print_in_middle (rapor_pencere, 1, 30, 22, _("Faturas�z �rsaliyeler Raporu"), COLOR_PAIR (1));
wrefresh(rapor_pencere);
sprintf(query, "select irsaliye_no, tarih, firma_adi, toplam_tutar from irsaliyeler where row_no>%d and row_no<%d and tur='0' order by irsaliye_no",
		basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

i=2;
for (j=0;j<pgsql_row_miktar;j++)
{	
	sprintf(yazi,"%-10.10s|", pgsql_kayit_field());
	sprintf(yazi,"%s%10.10s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%-30.30s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%25.25s|", yazi, yazidan_paraya_cevir(pgsql_kayit_field()) );
	
	mvwprintw (rapor_pencere,i, 1, "%.78s", yazi);

i++;	
wrefresh(rapor_pencere);
if (i>23)	{i=2;while (1){ if (getch()==32); break;} }
	
}
while (1){ if (getch()==32); break;}
delwin(rapor_pencere);
refresh();

}

else
{
	//yaziciya gonder secili ise
	
	yazici=fopen("/opt/acikisletme/conf/faturasizirsaliyeler","w");
	
	tarih_ayarla();
	sprintf(baslik_yazisi,
	_("%s �irketi - Faturas� Kesilmemi� Irsaliyeler D�k�m� - Rapor Zaman�: %s/%s/%s  %s:%s:%s - kullan�c�: %s\n\n"),
	sirket_adi, gun, ay, yil, saat, dakika, saniye, kullanici);
	fputs(baslik_yazisi, yazici);
		
	fputs(_("                                                        Faturas�z Irsaliyeler D�k�m�\n"), yazici);
	fputs("____________________________________________________________________________________________________________________________________________\n",yazici);
	
sprintf(deger,
"|%5.5s|%10.10s|%-15.15s|%-15.15s|%-25.25s|%25.25s|%16.16s|%-20.20s|\n",
_("No."), _("Tarih"), _("�r�n Kodu"), _("Irsaliye No."), _("�r�n Ad�"), _("Birim Fiyat"), _("Miktar"), _("Giden Firma Ad�")   );
fputs(deger, yazici);
	
fputs("____________________________________________________________________________________________________________________________________________\n",yazici);
	
sprintf(query,
"select row_no, tarih, malzeme_kodu, belge_no, aciklama, birim_fiyat, giden, birim_tip, kime  from stokgiden where row_no>%d and row_no<%d and fatura_no='' order by row_no",
basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
i=0;
for (j=0;j<pgsql_row_miktar;j++)
    {	
sprintf(alan1, "%s", pgsql_kayit_field());
sql_tarih_ayir(pgsql_kayit_field());

sprintf(alan3, "%s", pgsql_kayit_field());
sprintf(alan4, "%s", pgsql_kayit_field());
sprintf(alan5, "%s", pgsql_kayit_field());
sprintf(alan6, "%s", pgsql_kayit_field());
sprintf(alan7, "%s", pgsql_kayit_field());
sprintf(alan8, "%s", pgsql_kayit_field());
sprintf(alan9, "%s", pgsql_kayit_field());

sprintf(deger,
"|%5.5s|%2.2s/%2.2s/%4.4s|%-15.15s|%-15.15s|%-25.25s|%25.25s|%10.10s %5.5s|%20.20s|\n",
alan1, sql_gun, sql_ay, sql_yil, alan3, alan4, alan5, alan6, alan7, alan8, alan9    );

fputs(deger, yazici);
i++;
//if (i>50) fputc(12,yazici);
		
	}
		
	fclose( yazici );
	
	system("enscript --quiet --encoding=88599 --font=Courier8 /opt/acikisletme/conf/faturasizirsaliyeler -B -r -MA4dj");
	system("rm -rf /opt/acikisletme/conf/faturasizirsaliyeler");		
	
	mesaj( _("Report sended to printer.") );

}
	
//bitis	
}


/************************************************************/

int borc_alacak_raporu()
{

FIELD *field[3];
FORM  *my_form;
int ch,i;
int basno;
int bitno;
char *deger[20][60];

touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 6, 25, 0, 0);
field[1] = new_field(1, 10, 6, 65, 0, 0);
field[2] = new_field(1, 1, 10, 20, 0, 0);
field[3] = new_field(1, 1, 12, 20, 0, 0);
field[4] = NULL;

for (i=0;i<5;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                               Bor�/Alacak Listesi                             ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(6, 1, _("Ba�lang�� kay�t no :") );
mvprintw(6, 40, _("Biti� kay�t no    :") );

mvprintw(8, 1, _("��kt� i�in kullan�lacak olan ayg�t� se�iniz.") );
mvprintw(10, 1, _("Ekran   : ") );
mvprintw(12, 1, _("Yaz�c�  : ") );

//koseli parantezler koyulacak

mvprintw(6, 24, "[          ]");
mvprintw(6, 64, "[          ]");
mvprintw(10, 19, "[ ]");
mvprintw(12, 19, "[ ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("      F5 -> Rapor                                 ESC-> ��k��           ") );
	
mvprintw(LINES - 7, 0, _(" - �stenen ba�lang�� ve biti� kay�t numaralar� yaz�lmal�d�r.") );
mvprintw(LINES - 6, 0, _(" - Rapor ��kt�s� ayg�t se�imi bo�luk tu�una basarak yap�n�z.") );

	
	
	/*ba�lang�� ve biti de�erleri �ncednen ilk ve son kay�t yaz�lacak*/
	set_field_buffer(field[0], 0,  "1");
	row_no_buyuk("firmalar");
	set_field_buffer(field[1], 0,  rowno);
	
	set_field_buffer(field[2], 0,  "X");
	set_field_buffer(field[3], 0,  "");
		
	refresh();
	
while ( (ch!= 27)  )
{      
ch=getch();
	switch(ch)
		{			
		case KEY_F(10):
			ch=27;
			break;
		case 8: /*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE:
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		case 9: /*TAB tusuna bas�lmas� durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tu�una bas�lmas� durumunda sonraki feild gececek*/
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
		case  KEY_F(5): /*rapor ��kt�s�*/
						
			form_driver(my_form, REQ_NEXT_FIELD);		
			basno=atoi( rtrim(field_buffer(field[0],0)," ") );			
			bitno=atoi( rtrim(field_buffer(field[1],0)," ") );			
			sprintf(deger,"%s", rtrim(field_buffer(field[2],0),"")  );
		
			if (strstr(deger,"X")) borc_alacak_raporu_yazdir( 0, basno, bitno);
			
			sprintf(deger,"%s", rtrim(field_buffer(field[3],0),"")  );
			if (strstr(deger,"X")) borc_alacak_raporu_yazdir( 1, basno, bitno);
									
			refresh();
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_PREV_FIELD);			
			
			break;						
		case 32: //space tusu 32 kodunu i�ermektedir.
			/*ekran yaz�c� bolumu s�f�rlanmal�d�r*/
			set_field_buffer(field[2], 0,  "");
			set_field_buffer(field[3], 0,  "");
			form_driver(my_form, 88);		//88 b�y�k X tu�una kar��l�k gelmektedir.
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}

}

for (i = 0; i < 4; i++);  free_field (field[i]);	
}


/************************************************************/

int borc_alacak_raporu_yazdir(int cikti,int basno, int bitno)
{
	
char *deger[20][60];
int i,j;
WINDOW *rapor_pencere;

char tutar_yazisi2[30]="";
char baslik_yazisi[250];
char sirket_adi[100];

long long int borc_tutar=0;
long long int alacak_tutar=0;
char tutar_yazi[30]="";
char durum_yazi[5]="";	

//Firma ad� al�nacak 
sprintf(query, "select sirket_adi from sirket");

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

sprintf(sirket_adi,"%s", pgsql_kayit_field() );
	
if (cikti==0)//0 ise ekran
{	
rapor_pencere = newwin (24, 80, 0, 0);
keypad (rapor_pencere, TRUE);
box (rapor_pencere, 0, 0);
print_in_middle (rapor_pencere, 1, 30, 22, _("Bor� Alacak Listesi"), COLOR_PAIR (1));
wrefresh(rapor_pencere);
sprintf(query, "select bilanco_no, firma_adi, telefon, bakiye, durum from firmalar where row_no>%d and row_no<%d order by row_no",
		basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

i=2;
for (j=0;j<pgsql_row_miktar;j++)
{	
	sprintf(yazi,"%6.6s|", pgsql_kayit_field());
	sprintf(yazi,"%s%25.25s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%15.15s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%25.25s|", yazi, yazidan_paraya_cevir(pgsql_kayit_field()) );
	sprintf(yazi,"%s%2.2s|", yazi, pgsql_kayit_field());	
	
	mvwprintw (rapor_pencere,i, 1, "%.78s", yazi);

i++;	
wrefresh(rapor_pencere);
if (i>23)	{i=2;while (1){ if (getch()==32); break;} }
	
}
while (1){ if (getch()==32); break;}
delwin(rapor_pencere);
refresh();

}

else
{
	//yaziciya gonder secili ise
	
	yazici=fopen("/opt/acikisletme/conf/borcalacak","w");
	
	//yazici_yatay_dikey(yatay);
	
//ilk olarak sayfan�n en ba��na uygulama ad� s�r�m� ve sol k��eyede tarih saat atal�m
	
	tarih_ayarla();
	sprintf(baslik_yazisi,
	_("%s �irketi  Bor�/Alacak Durum Raporu - Rapor Zaman�: %s/%s/%s   %s:%s:%s\n\n"),
	sirket_adi, gun, ay, yil, saat, dakika, saniye);
	fputs(baslik_yazisi, yazici);
		
	fputs(_("                                                                     Bor� - Alacak Durum Raporu \n"), yazici);	
fputs("____________________________________________________________________________________________________________________________________________________________\n",yazici);
	
sprintf(deger,
"|%5.5s|%15.15s|%-35.35s|%-15.15s|%25.25s|%25.25s|%25.25s|\n",
_("No.") , _(" F.No."), _("Firma Ad�"), _("Telefon"), _("A��klama"), _("Bor�"), _("Alacak") );

fputs("____________________________________________________________________________________________________________________________________________________________\n",yazici);

fputs(deger, yazici);
sprintf(query, "select row_no, bilanco_no, firma_adi, telefon, aciklama, bakiye, durum from firmalar where row_no>%d and row_no<%d order by row_no", basno-1, bitno+1);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
i=0;
for (j=0;j<pgsql_row_miktar;j++)
   {
sprintf(alan1, "%s", pgsql_kayit_field());
sprintf(alan2, "%s", pgsql_kayit_field());
sprintf(alan3, "%s", pgsql_kayit_field());
//sprintf(alan4, "%s", pgsql_kayit_field());
sprintf(alan5, "%s", pgsql_kayit_field());
//sprintf(alan6, "%s", pgsql_kayit_field());
sprintf(alan7, "%s", pgsql_kayit_field());
sprintf(alan8, "%s", pgsql_kayit_field());
sprintf(alan9, "%s", pgsql_kayit_field());

	//bu alanda bor� alacak durumuna g�re gerekli yere yazma i�lemi yap�l�yor
	sprintf(durum_yazi, "%s", alan9  );
   
	//eger alacak ise bor� hanesine �izgi �ekilecek
	if (strstr(durum_yazi,"A"))
		{
		strcpy(tutar_yazi,"");
		strcpy(tutar_yazi, alan8 );
		alacak_tutar=alacak_tutar+atoll(tutar_yazi);
		sprintf(deger,
		"|%5.5s|%15.15s|%-35.35s|%-15.15s|%25.25s|%25.25s|%25.25s|\n",
		alan1, alan2, alan3, /*alan4,*/ alan5, /*alan6, alan7,*/  "  -------------",
		yazidan_paraya_cevir( tutar_yazi ) );
			
		fputs(deger, yazici);		
		i++;

	}

	//eger borc ise
	if (strstr(durum_yazi, "B"))
		{
		strcpy(tutar_yazi,"");
		strcpy(tutar_yazi, alan7 );
		borc_tutar=borc_tutar+atoll(tutar_yazi);
		sprintf(deger,
		"|%5.5s|%15.15s|%-35.35s|%-15.15s|%25.25s|%25.25s|%25.25s|\n",
		alan1, alan2, alan3, /*alan4,*/ alan5, /*alan6, alan7,*/ yazidan_paraya_cevir( tutar_yazi ),
		 "  -------------"	 );
				
		fputs(deger, yazici);		
		i++;

		}

	//if (i>40) fputc(12,yazici);
	
	}
	
	//genel toplamlar� yaz
		
	fputs(deger, yazici);
	
	sprintf(tutar_yazi,"%lli",borc_tutar);
	sprintf(tutar_yazisi2,"%lli",alacak_tutar);

fputs("____________________________________________________________________________________________________________________________________________________________\n",yazici);	
	
//ayni sprintf icinde iki adet yazdian paraya olursa hepside ilk degeri al�yor
	sprintf(query, "%s", yazidan_paraya_cevir(tutar_yazisi2) );
	sprintf(deger, "%100.100s  %25.25s %25.25s\n", _("Genel Toplamlar  :"),
			yazidan_paraya_cevir(tutar_yazi), query  );
	
	fputs(deger, yazici);
		
	//fark yazd�r
	if (borc_tutar>alacak_tutar){
	borc_tutar=borc_tutar-alacak_tutar;
	sprintf(tutar_yazi,"%lli",borc_tutar);
	sprintf(deger, "%100.100s  %25.25s %25.25s\n", _("Fark Toplam�   :"),
					yazidan_paraya_cevir(tutar_yazi) , "  -------------" );
		
	fputs(deger, yazici);
			
	}
		
	if (borc_tutar<alacak_tutar){
	alacak_tutar=alacak_tutar-borc_tutar;
	sprintf(tutar_yazi,"%lli", alacak_tutar);
	sprintf(deger, "%100.100s  %25.25s %25.25s\n",
	 "Fark Toplam�   :", "  -------------", yazidan_paraya_cevir(tutar_yazi));
		
	fputs(deger, yazici);
	
	}
	
	fclose( yazici );

	system("enscript --quiet --encoding=88599 --font=Courier8 /opt/acikisletme/conf/borcalacak -B -r  -MA4dj");
	system("rm -rf /opt/acikisletme/conf/borcalacak");
	
	mesaj( _("Report sended to printer.") );
		
}
	


//bitis	
}

/************************************************************/

int adres_etiket_raporu()
{
FIELD *field[7];
FORM  *my_form;
WINDOW *my_menu_win;
int ch,i;
int basno;
int bitno;
int etiket_tip;
int sirada_etiket_miktar;
int sirket_etiketi_miktar;
	
touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 5, 25, 0, 0);
field[1] = new_field(1, 10, 5, 65, 0, 0);
field[2] = new_field(1, 1, 10, 40, 0, 0);
field[3] = new_field(1, 1, 12, 40, 0, 0);
field[4] = new_field(1, 1, 15, 40, 0, 0);
field[5] = new_field(1, 3, 17, 70, 0, 0);
field[6] = NULL;

for (i=0;i<7;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                 M��teri/�irket Adres Etiket Bas�m�                             ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(3, 1, _("M��teri etiketi se�ilmesi durumunda hangi kay�t aral�klar�") );
mvprintw(5, 1, _("Ba�lang�� kay�t no :") );
mvprintw(5, 40, _("Biti� kay�t no    :") );

attron(A_UNDERLINE);
mvprintw(6, 0, "                                                                               ");
attroff(A_UNDERLINE);

mvprintw(8, 1,  _("Kendi �irketimizin etiketi veyahutta m��terilerin etiketimi bas�lacak.?") );
mvprintw(10, 1, _("M��teri etiketimi bas�ls�n.? : ") );
mvprintw(12, 1, _("�irket etiketimi bas�ls�n.?  : ") );

attron(A_UNDERLINE);
mvprintw(13, 0, "                                                                               ");
attroff(A_UNDERLINE);

mvprintw(15, 1, _("Bir s�rada ka� etiket var.?  : ") );

mvprintw(17, 1, _("E�er �irketimizin etiketi bas�lacaksa ka� adet bas�ls�n.?   : ") );

mvprintw(5, 24, "[          ]");
mvprintw(5, 64, "[          ]");

mvprintw(10, 39, "[ ]");
mvprintw(12, 39, "[ ]");
mvprintw(15, 39, "[ ]");
mvprintw(17, 69, "[   ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("      F5 -> Yazd�r                                ESC-> ��k��           ") );
mvprintw(LINES - 5, 0, _(" - M��teri ve �irket etiketlerini bo�luk tu�una basarak se�iniz.") );
	
	/*ba�lang�� ve biti de�erleri �ncednen ilk ve son kay�t yaz�lacak*/
	set_field_buffer(field[0], 0,  "1");
	row_no_buyuk("firmalar");
	set_field_buffer(field[1], 0,  rowno);
	
	set_field_buffer(field[2], 0,  "X");
	set_field_buffer(field[3], 0,  "");
	set_field_buffer(field[4], 0,  "2");
	
	refresh();
	
while ( (ch!= 27)  )
{      
ch=getch();
	switch(ch)
		{			
		case KEY_F(10):
			ch=27;
			break;
		case 8: /*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case KEY_BACKSPACE:
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case 9: /*TAB tusuna bas�lmas� durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tu�una bas�lmas� durumunda sonraki feild gececek*/
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
		case  KEY_F(5): /*rapor ��kt�s�*/
						
			form_driver(my_form, REQ_NEXT_FIELD);
		
			//degerler duzenleniyor
			basno=atoi( g_strstrip(field_buffer(field[0],0)) );
			bitno=atoi( g_strstrip(field_buffer(field[1],0)) );
			etiket_tip=0;
		    if ( strstr(g_strstrip(field_buffer(field[2],0)),"X") ) etiket_tip=0;//musteri
			if ( strstr(g_strstrip(field_buffer(field[3],0)),"X") )  etiket_tip=1;//satici

			sirada_etiket_miktar=atoi( g_strstrip(field_buffer(field[4],0)) );
			sirket_etiketi_miktar=atoi( g_strstrip(field_buffer(field[5],0)) );
			adres_etiket_raporu_yazdir(basno, bitno, etiket_tip, sirada_etiket_miktar, sirket_etiketi_miktar);
			mesaj( _("Sended to printer") );
			refresh;
			break;						
		case 32: //space tusu 32 kodunu i�ermektedir.
			/*ekran yaz�c� bolumu s�f�rlanmal�d�r*/
			set_field_buffer(field[2], 0, " ");
			set_field_buffer(field[3], 0, " ");
			form_driver(my_form, 88);		//88 b�y�k X tu�una kar��l�k gelmektedir.
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}


}

for (i = 0; i < 7; i++);  free_field (field[i]);	
}

/************************************************************/

int adres_etiket_raporu_yazdir(int basno, int bitno, int etiket_tip, int sirada_etiket_miktar, int sirket_etiketi_miktar)
{
int miktar;
char sirket_adi_str[40];
char adres_str[50];	
	
	switch (etiket_tip)
	{
		case 0://musteri etiketleri bas�m�
			return;
		
		case 1://sirket etiketi bas�m�
		
			sprintf(query,"select sirket_adi, adres, vergi_dairesi, vergi_no, telefon1 from sirket");
			if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			sprintf( sirket_adi_str, "%s", pgsql_kayit_field() );
			sprintf( adres_str, "%s\n", pgsql_kayit_field() );
			
			switch (sirada_etiket_miktar)
			{
				case 2:
				miktar=(sirket_etiketi_miktar/sirada_etiket_miktar);

				yazici=fopen("/opt/acikisletme/conf/etiket","w");
						
				for (i=0;i<miktar;i++)
				{
				fputs(sirket_adi_str, yazici);
				fputs("\n",yazici);
				fputs(adres_str, yazici);
				
				fputs("\n",yazici);
				fputs("\n",yazici);
					
				}

				fclose( yazici );		
				
				system("enscript --quiet --encoding=88599 --font=Courier8 /opt/acikisletme/conf/etiket -B -2R  -MA4");
				system("rm -rf /opt/acikisletme/conf/etiket");
				
				return;
				
				case 3:
				
				return;
				
			}
						
		return;
			
			
	}
	

}



/************************************************************/
int cek_hareket_raporu()
{

FIELD *field[3];
FORM  *my_form;
int ch,i;
int basno;
int bitno;
char *deger[20][60];

touchwin(ana_win);
wrefresh(ana_win);

field[0] = new_field(1, 10, 6, 25, 0, 0);
field[1] = new_field(1, 10, 6, 65, 0, 0);
field[2] = new_field(1, 1, 10, 20, 0, 0);
field[3] = new_field(1, 1, 12, 20, 0, 0);
field[4] = NULL;

for (i=0;i<5;i++)
{
field_opts_off(field[i], O_AUTOSKIP); 
}

form_driver(field[0],REQ_FIRST_PAGE);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                        iki tarih aras� Cek/Senet Hareketleri                   ") );
attroff(COLOR_PAIR(1));	

attron(COLOR_PAIR(2));
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(6, 1, _("Ba�lang�� tarih :") );
mvprintw(6, 40, _("Biti� tarih no    :") );

mvprintw(8, 1, _("��kt� i�in kullan�lacak olan ayg�t� se�iniz.") );
mvprintw(10, 1, _("Ekran   : ") );
mvprintw(12, 1, _("Yaz�c�  : ") );

//koseli parantezler koyulacak

mvprintw(6, 24, "[          ]");
mvprintw(6, 64, "[          ]");
mvprintw(10, 19, "[ ]");
mvprintw(12, 19, "[ ]");

baslik_goruntule();
mvprintw(LINES - 3, 0, _("      F5 -> Rapor                                 ESC-> ��k��           ") );
	
mvprintw(LINES - 7, 0, _(" - �stenen ba�lang�� ve biti� kay�t numaralar� yaz�lmal�d�r.") );
mvprintw(LINES - 6, 0, _(" - Rapor ��kt�s� ayg�t se�imi bo�luk tu�una basarak yap�n�z.") );

set_field_buffer(field[0], 0,  "01.01.2000");
set_field_buffer(field[1], 0,  "01.01.2099");
	
set_field_buffer(field[2], 0,  "X");
set_field_buffer(field[3], 0,  "");
		
refresh();
	
while ( (ch!= 27)  )
{      
ch=getch();
	switch(ch)
		{			
		case KEY_F(10):
			ch=27;
			break;
		case 8: /*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;

		case KEY_BACKSPACE:
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		
		case 9: /*TAB tusuna bas�lmas� durumunda sonraki field konumlan*/
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_END_LINE);
			break;		
		case 10: /*ENTER tu�una bas�lmas� durumunda sonraki feild gececek*/
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
		case  KEY_F(5): /*rapor ��kt�s�*/
						
			form_driver(my_form, REQ_NEXT_FIELD);		
					
			sprintf(deger,"%s", rtrim(field_buffer(field[2],0),"")  );
		
			if (strstr(deger,"X")) 
				cek_hareket_raporu_yazdir( 0, rtrim(field_buffer(field[0],0)," ") ,
				rtrim(field_buffer(field[1],0)," ")  );
			
			sprintf(deger,"%s", rtrim(field_buffer(field[3],0),"")  );
			if (strstr(deger,"X")) 
				cek_hareket_raporu_yazdir( 1, rtrim(field_buffer(field[0],0)," ") ,
				rtrim(field_buffer(field[1],0)," ")  );
			
									
			refresh();
			form_driver(my_form, REQ_NEXT_FIELD);
			form_driver(my_form, REQ_PREV_FIELD);			
			
			break;						
		case 32: //space tusu 32 kodunu i�ermektedir.
			/*ekran yaz�c� bolumu s�f�rlanmal�d�r*/
			set_field_buffer(field[2], 0,  "");
			set_field_buffer(field[3], 0,  "");
			form_driver(my_form, 88);		//88 b�y�k X tu�una kar��l�k gelmektedir.
		break;
		
		default:
			form_driver(my_form, ch);
			break;
				
		}

}

for (i = 0; i < 4; i++);  free_field (field[i]);	
	
}

/************************************************************/
int cek_hareket_raporu_yazdir(char  cikti[20],char basno[20], int bitno)
{

char *deger[20][60];
int i,j;
WINDOW *rapor_pencere;

char tutar_yazisi2[30]="";
char baslik_yazisi[250];
char sirket_adi[100];

long long int borc_tutar=0;
long long int alacak_tutar=0;
char tutar_yazi[30]="";
char durum_yazi[5]="";	

//Firma ad� al�nacak 
sprintf(query, "select sirket_adi from sirket");

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

sprintf(sirket_adi,"%s", pgsql_kayit_field() );
	
if (cikti==0)//0 ise ekran
{	
rapor_pencere = newwin (24, 80, 0, 0);
keypad (rapor_pencere, TRUE);
box (rapor_pencere, 0, 0);
print_in_middle (rapor_pencere, 1, 30, 22, _("iki tarih aras� �ek hareketleri"), COLOR_PAIR (1));
wrefresh(rapor_pencere);
sprintf(query, "select cek_no, cek_tarih, firma_adi, cikan_firma_adi, durum from ceksenet where islem_tarih>='%s' and islem_tarih<='%s' order by cek_tarih",
		basno, bitno);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

i=2;
for (j=0;j<pgsql_row_miktar;j++)
{	
	sprintf(yazi,"%-15.15s|", pgsql_kayit_field());
	sprintf(yazi,"%s%10.10s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%-20.20s|", yazi, pgsql_kayit_field());
	sprintf(yazi,"%s%-20.20s|", yazi, yazidan_paraya_cevir(pgsql_kayit_field()) );
	sprintf(yazi,"%s%6.6s|", yazi, pgsql_kayit_field());	
	
	mvwprintw (rapor_pencere,i, 1, "%.78s", yazi);

i++;	
wrefresh(rapor_pencere);
if (i>23)	{i=2;while (1){ if (getch()==32); break;} }
	
}
while (1){ if (getch()==32); break;}
delwin(rapor_pencere);
refresh();

}

else
{
	//yaziciya gonder secili ise
	
	yazici=fopen("/opt/acikisletme/conf/cekhareket","w");
	
	//yazici_yatay_dikey(yatay);
	
//ilk olarak sayfan�n en ba��na uygulama ad� s�r�m� ve sol k��eyede tarih saat atal�m
	
	tarih_ayarla();
	sprintf(baslik_yazisi,
	_("%s �irketi �ek/Senet Hareketleri Raporu - Rapor Zaman�: %s/%s/%s  %s:%s:%s\n\n"),
	sirket_adi, gun, ay, yil, saat, dakika, saniye);
	fputs(baslik_yazisi, yazici);
		
	fputs(_("                                                                  �ek - Senet Hareketleri Raporu \n"), yazici);	
	fputs("___________________________________________________________________________________________________________________________________________________________\n",yazici);
	
sprintf(deger,
"|%10.10s|%10.10s|%-20.20s|%17.17s|%-10.10s|%10.10s|%10.10s|%15.15s|%15.15s|%6.6s|%20.20s|\n",
_("Makbuz No."), _("islem tarih."), _("Firma Ad�"), _("Tutar"), _("Banka Ad�"), _("�ek no"), _("�ek Tarih"), _("Keside Yeri"), _("�ek Kesen Firma"), _("Durum"),
_("Kime Verildi") );
fputs(deger, yazici);
fputs("___________________________________________________________________________________________________________________________________________________________\n",yazici);

sprintf(query, "select belge_no, islem_tarih, firma_adi, tutar, banka_adi, cek_no, cek_tarih, keside_yer, cek_kesen_firma, durum, cikan_firma_adi from ceksenet where islem_tarih>='%s' and islem_tarih<='%s' order by islem_tarih", basno, bitno);
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
i=0;
  for (j=0;j<pgsql_row_miktar;j++)
    {
sprintf(alan1, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan2, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan3, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan4, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan5, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan6, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan7, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan8, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan9, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan10, "%s", g_strstrip(pgsql_kayit_field()));
sprintf(alan11, "%s", g_strstrip(pgsql_kayit_field()));
	
sprintf(deger,
"|%10.10s|%10.10s|%-20.20s|%17.17s|%-10.10s|%10.10s|%10.10s|%15.15s|%15.15s|%6.6s|%20.20s|\n",
alan1, alan2, alan3, yazidan_paraya_cevir(alan4), alan5, alan6, alan7, alan8, alan9, alan10, alan11  
		);

fputs(deger, yazici);		
	
	}
	

	fputs(_("\n\nNot: Durum hanesi a��klamas�\n�irket : �ek �u anda elimizde , Firma : Bir firmaya ��k��� yap�lm��,  Banka : Tahsilat� taraf�m�zdan yap�lm��\n"), yazici);
		
	fclose( yazici );

	system("enscript --quiet --encoding=88599 --font=Courier8 /opt/acikisletme/conf/cekhareket -B -r  -MA4dj");
	system("rm -rf /opt/acikisletme/conf/cekhareket");
	
	mesaj( _("Report sended to printer.") );
		
}
	


//bitis	
}

