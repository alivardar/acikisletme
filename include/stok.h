
int stok()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
	WINDOW *my_menu_win;
	int n_choices, i;
	ITEM *cur_item;	
	
char *choices_stok[] = {
	    _("Incoming waybill"),
	    _("Outgoing waybill"),
	    _("New stock account"),
	    _("Stock accounts"),
	    _("Back"),
         (char *)NULL,
                  };
	
  (int) signal (SIGINT, sonlandir);
  (int) signal (SIGILL, sonlandir);
  (int) signal (SIGTERM, sonlandir);	
	
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);

        ekran_temizle();
	baslik_goruntule();
		
	n_choices = ARRAY_SIZE(choices_stok);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
	my_items[i] = new_item(choices_stok[i], " ");

	my_menu = new_menu((ITEM **)my_items);

	my_menu_win = newwin(15, 50, 5, 10);
	keypad(my_menu_win, TRUE);

	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 10, 40, 4, 2));

	set_menu_mark(my_menu, mark);

	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 45, _("Stock List"), COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 48);
	mvwaddch(my_menu_win, 2, 49, ACS_RTEE);
        
	post_menu(my_menu);
	
touchwin(my_menu_win);
wrefresh(my_menu_win);

while ( (c!= 27)  )
	{      
		c=wgetch(my_menu_win);
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
					case 1:	/*yeni stok girisi*/
						irsaliye_giris();

    					baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
					case 2:	/*eski stok girisi*/
						irsaliye_cikis();

    					baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
					case 3:	/*yeni stok kartonu*/
						stok_karton_yeni();    

    					baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
					case 4:	/*eski stok kartonu*/
						stok_karton_eski(0);
						
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);						
						break;
					case 5:	/*ana menu*/

						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
		
				        for(i = 0; i<n_choices; ++i)  free_item(my_items[i]);
					
					return 0;

				        }
                                       break;
				
				refresh();
				pos_menu_cursor(my_menu);
		}
                wrefresh(my_menu_win);
		
		
	}	

for(i = 0; i < n_choices; ++i)  free_item(my_items[i]);
	
return 0;
}


/******************************************************************************/

int stok_karton_yeni()
{

char query[2000]="";

FIELD *field[9];
FORM  *my_form;
int ch,i;

touchwin(ana_win);
wrefresh(ana_win);

ekran_temizle();

//set_field_type(field[2], TYPE_ALNUM);
//set_field_type(field[6], TYPE_INTEGER);

/*Kay�t i�elmeinden sonra row no de�eri verilir ekrana yaz�l�r*/

/*stokkart tablosu*/
/*row_no*/
field[0] = new_field(1, 8, 4, 5, 0, 0);
/*malzeme_kodu*/
field[1] = new_field(1, 40, 4, 30, 0, 0);
/*malzeme_adi*/
field[2] = new_field(1, 60, 6, 15, 0, 0);
/*aciklama*/
field[3] = new_field(1, 65, 8, 10, 0, 0);
/*Seri no*/
field[4] = new_field(1, 50, 10, 20, 0, 0);
/*birim_fiyat*/
field[5] = new_field(1, 50, 12, 20, 0, 0);
/*mevcut*/
field[6] = new_field(1, 50, 14, 20, 0, 0);
/*kdv oran�*/
field[7] = new_field(1, 3, 16, 20, 0, 0);
//birim_tip
field[8] = new_field(1, 5, 18, 20, 0, 0);
field[9] = NULL;

for (i=0;i<9;i++)
{
set_field_fore(field[i], COLOR_PAIR(2));
set_field_back(field[i], COLOR_PAIR(2));
field_opts_off(field[i], O_AUTOSKIP); 
/*hiazlama fonksiyonu*/
set_field_just(field[i], JUSTIFY_RIGHT);
}

field_opts_off(field[0],O_ACTIVE);
form_driver(field[1],REQ_FIRST_PAGE);

//kdv alan�na int gitilmel zorunda
set_field_type (field[7], TYPE_INTEGER,0);
//para alan�na numeric
set_field_type (field[5], TYPE_INTEGER,0);
//mevcut alan�n
set_field_type (field[6], TYPE_INTEGER,0);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(2));
mvprintw(1, 0, _("                           New Stock Account                                    ") );
baslik_goruntule();
attroff(COLOR_PAIR(2));	

/*toplam 3 sat�r*/
mvprintw(4, 1, _("No:") );
mvprintw(4, 15, _("Material code :") );
mvprintw(6, 1, _("Material name:") );
mvprintw(8, 1, _("Explanation:") );
mvprintw(10, 1, _("Serial:") );
mvprintw(12, 1, _("Unit price:") );
mvprintw(14, 1, _("Inhand:") );
mvprintw(16, 1, _("Tax percent:") );
mvprintw(18, 1, _("Unit of measurement:") );


mvprintw(LINES - 3, 0, _("                  F2->Save     F3->New   ESC->Exit " ) );

/*row_no alan� ile ilgili de�er ekranda yaz�lmas� gereken yere 
sql server i�erisinden okunarak yaz�lacakt�r*/

row_no_buyuk("stokkart");
set_field_buffer(field[0], 0, rowno );

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
			form_driver(my_form, REQ_PREV_FIELD);
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
			
		case KEY_F(2):

/*Kay�t yapmadan evvel snaroki alana gecilmeli okuma yapm�yor aksi halde*/
form_driver(my_form, REQ_NEXT_FIELD);

sprintf(query, 
"insert into stokkart (malzeme_kodu, malzeme_adi, aciklama, seri_no, birim_fiyat, mevcut, kdv_orani, birim_tip) values ('%s' ,'%s' ,'%s' ,'%s' ,'%s' ,'%s','%s','%s' )",
/*row_no silindi, malzeme_kodu*/
g_strstrip(field_buffer(field[1],0)),
/*malzeme_adi, aciklama*/
g_strstrip(field_buffer(field[2], 0)), g_strstrip(field_buffer(field[3],0)),
/*seri_no, birim_fiyat*/
g_strstrip(field_buffer(field[4],0)),  g_strstrip(field_buffer(field[5],0)),
/*mevcut,  kdv orani*/
g_strstrip(field_buffer(field[6],0)), g_strstrip(field_buffer(field[7],0)),
//biri_tip
g_strstrip(field_buffer(field[8],0))   );
	
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

/*stok kay�d� yap�ld�ktan sonra kullan�c� yeni kay�t girmek isteyebilir o sorulacak ve 
	sonras�nda field alanlar� bosalt�lacak*/
	
		donen_deger=onayla(yeni_kayit_istiyormusunuz);
		if (donen_deger == 0)  return;
		if (donen_deger==1)
			{
			/*evet de�eri girildi*/				
				/*row_no alan� ogrenilerek 1 art�l�p ekrana yaz�lacakt�r*/
				//row_no_buyuk("stokkart");
				//set_field_buffer(field[0], 0, rowno );
			/*diger alanlar ise bosalt�lacakt�r*/
				set_field_buffer(field[1], 0, "" );
				set_field_buffer(field[2], 0, "" );
				set_field_buffer(field[3], 0, "" );
				set_field_buffer(field[4], 0, "" );
				set_field_buffer(field[5], 0, "" );
				set_field_buffer(field[6], 0, "" );
				set_field_buffer(field[7], 0, "" );	
				set_field_buffer(field[8], 0, "" );	
			}
		
			break;
/*f2 kay�t i�lemlerinin sonu*/

			
		case KEY_F(3):/*f3 yeni i�lemleri i�in*/
	
		donen_deger=onayla(yeni_kayit);
		if (donen_deger==1)
			{
			/*evet de�eri girildi*/				
				/*row_no alan� ogrenilerek 1 art�l�p ekrana yaz�lacakt�r*/
				//row_no_buyuk("stokkart");
				//set_field_buffer(field[0], 0, rowno );
			/*diger alanlar ise bosalt�lacakt�r*/
				set_field_buffer(field[1], 0, "" );
				set_field_buffer(field[2], 0, "" );
				set_field_buffer(field[3], 0, "" );
				set_field_buffer(field[4], 0, "" );
				set_field_buffer(field[5], 0, "" );
				set_field_buffer(field[6], 0, "" );
				set_field_buffer(field[7], 0, "" );
				
			}
					
			break;
			
		default:
			form_driver(my_form, ch);
			break;
	}

}


for (i = 0; i < 9; i++);  free_field (field[i]);
}

/******************************************************************************/

int stok_karton_eski(unsigned long int kayit_no)
{
char query[2000]="";

char gecici_yazi[1000]="";
	
FIELD *field[7];
FORM  *my_form;
int ch,i,n_choices,c;

ITEM *my_items[20];
ITEM *cur_item;
MENU *my_menu;
WINDOW *my_menu_win;

char *deger[20][60];
char *deger2[20][60];

char donen[60];
int *donen_deger;
char *yazi;

touchwin(ana_win);
wrefresh(ana_win);

ekran_temizle();

row_no_buyuk("stokkart");
if (row_no<kayit_no) kayit_no=kayit_no-15;

sprintf(query, 
"select row_no, malzeme_adi, malzeme_kodu, mevcut, birim_tip from stokkart order by malzeme_adi limit 15 offset %d",
kayit_no);

if ( strlen( rtrim(giris," ")) >2) sprintf (query,
"select row_no, malzeme_adi, malzeme_kodu, mevcut, birim_tip from stokkart where malzeme_adi like '%%%s%%' order by malzeme_adi limit 15 offset 0",	
giris );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

  i=0;
	for (i=0;i<pgsql_row_miktar;i++)
    {
	sprintf(deger[i],"%s", pgsql_kayit_field() );
    sprintf(deger2[i],"%-25.25s - ", pgsql_kayit_field() );    
	strcat(deger2[i], pgsql_kayit_field() );
	sprintf(deger2[i], _("%s - mevcut: %s ") , deger2[i], pgsql_kayit_field() );
	strcat(deger2[i], pgsql_kayit_field() );
		
	my_items[i] = new_item( &deger[i], &deger2[i]);    
    }

	my_items[i] = (ITEM *)NULL;
	my_menu = new_menu((ITEM **)my_items);

	/*3 sat�r 3 sutun 15 sat�r 60 sutun pencere*/
    my_menu_win = newwin(21, 75, 1, 3);

    keypad(my_menu_win, TRUE);
     
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 15, 70, 4, 1));
	set_menu_format(my_menu, 15, 1);
			
    set_menu_mark(my_menu, mark);

	box(my_menu_win, 0, 0);
	//print_in_middle(my_menu_win, 1, 0, 75, _("  F4-> Edit   F5-> Find   F8-> Delete   ESC-> Exit    Please select "), COLOR_PAIR(1));

	print_in_middle (my_menu_win, 1, 0, 75,
	  _("  F4-> Edit F5-> Find  F8-> Delete  ESC-> Exit  Please select "), COLOR_PAIR (1));
	print_in_middle(my_menu_win, 20, 0, 75, ("  PAGE UP -> Prior Page     PAGE DOWN -> Next Page "), COLOR_PAIR(1));
	
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 80);
	mvwaddch(my_menu_win, 2, 79, ACS_RTEE);
	
	post_menu(my_menu);	
	wrefresh(my_menu_win);

/*buras� �al��maz ise hata veriyor ve de�eri okuyam�yor*/
menu_driver (my_menu, REQ_DOWN_ITEM);
cur_item = current_item (my_menu);
wrefresh (my_menu_win);

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
			case KEY_F(4):	/* F4 Stok kartonu duzenleme ekran� */				
				sprintf(gecici_yazi,"%s",item_name(cur_item) );
				stok_karton_duzenle(gecici_yazi);	
				return;
			break;			
			case KEY_F (5):	/*arama tusu bas�l�rsa */
	
				aranacak_kelime( _("�r�n ad� veya i�inde ge�en bir de�er giriniz.    D�zg�n s�ralama i�in bo� b�rak�n�z. ") );
				stok_karton_eski(0);
				return 0;	
			
			case KEY_F(8):/*sil tusu bas�l�rsa f8*/
				
				//silme hakk� kontrolu
				if ( haklari_kontrol_et(3)==0 )	{mesaj( _("You don't have delete permission.!!!") ); break;}
			
				donen_deger=onayla(kayit_silme_onayla);
				if (donen_deger==1)
					{
					/*Silme i�lemi onayland� ilgili olan row_no bulunacak ve kay�t silinecek daha
					sonra menu refresh edilmelidir*/
						
					//oncelikle bu kay�t ile ilgili islem yap�lm�s ise silme islemi iptal edilmelidir
					//stokgiden ve stokgelen tablolar�na bak�lacak
					
					sprintf(query, "select * from stokgiden where malzeme_kodu=(select malzeme_kodu from stokkart where row_no=%s)", item_name(cur_item));
					if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	
					if (pgsql_row_miktar>0)
					{mesaj ( _("Bu tablo �zerinde kay�t yap�lm��t�r. Silinemez.") ); 
						touchwin(my_menu_win);
						wrefresh(my_menu_win); break;}
					
					sprintf(query, "select * from stokgelen where malzeme_kodu=(select malzeme_kodu from stokkart where row_no=%s)", item_name(cur_item));
					if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	
					if (pgsql_row_miktar>0)
					{mesaj ( _("Bu tablo �zerinde kay�t yap�lm��t�r. Silinemez.") ); 
						touchwin(my_menu_win);wrefresh(my_menu_win);break;}					
										
					//sil	
					sprintf(query, "delete from stokkart where row_no='%s'", item_name(cur_item));
					
					if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
					
					unpost_menu(my_menu);
					free_menu(my_menu);
					for(c = 0; c < i; ++c)
					free_item(my_items[c]);
										
					stok_karton_eski(kayit_no);
					
					return ;
					}
					wrefresh(my_menu_win);
				break;
/*page up page down yaz�lacak*/
			
			case KEY_NPAGE: /* Page down bas�l�rsa*/
									
				kayit_no=kayit_no+15;
					
					unpost_menu(my_menu);
					free_menu(my_menu);
					for(c = 0; c < i; ++c)
					free_item(my_items[c]);
										
					stok_karton_eski(kayit_no);
					return;
				break;
						
			case KEY_PPAGE:/* Page up bas�l�rsa*/
				if (kayit_no>14) {kayit_no=kayit_no-15;}
			
				unpost_menu(my_menu);
				free_menu(my_menu);
				for(c = 0; c < i; ++c)
				free_item(my_items[c]);
				
					
				stok_karton_eski(kayit_no);
								
				return;
					
				break;
		}
	cur_item = current_item(my_menu);
	wrefresh(my_menu_win);
		}		
 
	/* uzerinde bulunan menu degerinini isim degeri yani ilk deger rowno*/
	//sprintf(donen,"%s",item_name(cur_item));

	unpost_menu(my_menu);
    free_menu(my_menu);

    //for(c = 0; c < i; ++c)    free_item(my_items[c]);

for (i = 0; i < 8; i++);  free_field (field[i]);
//return donen;
return;
}

/******************************************************************************/

int stok_karton_duzenle(unsigned long int kayit_no)
{
/*bu fonksiyon stok_karton_eski taraf�ndan menude duzenle t�klan�nca �a�r�l�r
gonderilen rowno ile duzenleme ekran� a��l�r*/
FIELD *field[9];
FORM  *my_form;
int ch;
char malzeme_adi_str[100] = "";
char malzeme_kodu_str[100] = "";

touchwin(ana_win);
wrefresh(ana_win);

ekran_temizle();

//set_field_type(field[2], TYPE_ALNUM);
//set_field_type(field[6], TYPE_INTEGER);

/*Kay�t i�elmeinden sonra row no de�eri verilir ekrana yaz�l�r*/

/*stokkart tablosu*/

/*row_no*/
field[0] = new_field(1, 8, 4, 5, 0, 0);
/*malzeme_kodu*/
field[1] = new_field(1, 40, 4, 30, 0, 0);
/*malzeme_adi*/
field[2] = new_field(1, 60, 6, 15, 0, 0);
/*aciklama*/
field[3] = new_field(1, 65, 8, 10, 0, 0);
/*Seri no*/
field[4] = new_field(1, 50, 10, 20, 0, 0);
/*birim_fiyat*/
field[5] = new_field(1, 50, 12, 20, 0, 0);
/*mevcut*/
field[6] = new_field(1, 50, 14, 20, 0, 0);
/*kdv oran�*/
field[7] = new_field(1, 3, 16, 20, 0, 0);
//biri_tip
field[8] = new_field(1, 5, 18, 20, 0, 0);
field[9] = NULL;

for (i=0;i<9;i++)
{
set_field_fore(field[i], COLOR_PAIR(2));
set_field_back(field[i], COLOR_PAIR(2));
field_opts_off(field[i], O_AUTOSKIP); 
set_field_just(field[i], JUSTIFY_RIGHT);
}

field_opts_off(field[0],O_ACTIVE);
form_driver(field[1],REQ_FIRST_PAGE);

//kdv alan�na int gitilmel zorunda
set_field_type (field[7], TYPE_INTEGER,0);
//para alan�na numeric
set_field_type (field[5], TYPE_INTEGER,0);
//mevcut alan�n
set_field_type (field[6], TYPE_INTEGER,0);

my_form = new_form(field);
post_form(my_form);

attron(COLOR_PAIR(1));
mvprintw(1, 0, _("                           New Stock Account                              ") );
baslik_goruntule();
attroff(COLOR_PAIR(1));	

/*toplam 3 sat�r*/
mvprintw(4, 1, _("No:") );
mvprintw(4, 15, _("Material code:") );
mvprintw(6, 1, _("Material name:") );
mvprintw(8, 1, _("Explanation:") );
mvprintw(10, 1, _("Serial no:") );
mvprintw(12, 1, _("Unit price:") );
mvprintw(14, 1, _("Inhand:") );
mvprintw(16, 1, _("Tax percent:") );
mvprintw(18, 1, _("�l�� Birimi:") );

mvprintw(LINES - 3, 0, _("                  F2-> Save   F3-> New  ESC-> Exit") );

/*kayit_no bilgiisi row_no bilgisidiri bu bilgi ile sql den bilgi �ekilecek ve 
field lar i�ine yerle�tirilecektir*/

 sprintf(query, "select row_no, malzeme_kodu, malzeme_adi, aciklama, seri_no, birim_fiyat, mevcut,kdv_orani, birim_tip  from stokkart where row_no='%s'", kayit_no);
 
 if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
 
 for (i=0;i<pgsql_row_miktar;i++)
    {
	set_field_buffer(field[0], 0, pgsql_kayit_field() );
	set_field_buffer(field[1], 0, pgsql_kayit_field() );
	set_field_buffer(field[2], 0, pgsql_kayit_field() );
	set_field_buffer(field[3], 0, pgsql_kayit_field() );
	set_field_buffer(field[4], 0, pgsql_kayit_field() );
	set_field_buffer(field[5], 0, pgsql_kayit_field() );		
	set_field_buffer(field[6], 0, pgsql_kayit_field() );		
	set_field_buffer(field[7], 0, pgsql_kayit_field() );
	set_field_buffer(field[8], 0, pgsql_kayit_field() );
	}
	
//eski malzeme_adi ve malzeme_kodu saklancak
sprintf(malzeme_kodu_str, "%s", g_strstrip(field_buffer (field[1], 0) ) );
sprintf(malzeme_adi_str, "%s", g_strstrip(field_buffer (field[2], 0) ) );	

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
			form_driver(my_form, REQ_PREV_FIELD);
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
			
		case KEY_F(2):

			/*Kay�t yapmadan evvel snaroki alana gecilmeli okuma yapm�yor aksi halde*/
			form_driver(my_form, REQ_NEXT_FIELD);

/* update i�lemi yap�l�yor */
sprintf(query, 
"update stokkart set malzeme_kodu='%s', malzeme_adi='%s', aciklama='%s', seri_no='%s', birim_fiyat='%s', mevcut='%s', kdv_orani='%s', birim_tip='%s' where (row_no='%s') ",
/*malzeme_kodu*/
g_strstrip(field_buffer(field[1],0)),
/*malzeme_adi, aciklama*/
g_strstrip(field_buffer(field[2], 0)), g_strstrip(field_buffer(field[3],0)),
/*seri_no, birim_fiyat*/
g_strstrip(field_buffer(field[4],0)),  g_strstrip(field_buffer(field[5],0)),
/*mevcut */
g_strstrip(field_buffer(field[6],0)),
//kdv orani, birim_tip
g_strstrip(field_buffer(field[7],0)), g_strstrip(field_buffer(field[8],0)),
g_strstrip(kayit_no)
	);

	if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//diger tablolar uzerinde de�i�klik yap�laml� malzeme_adi ve malzeme_kodu

//faturagelen tablosu
sprintf (query, "update faturagelen set malzeme_kodu='%s', aciklama='%s' where malzeme_kodu='%s' and aciklama='%s'", 
g_strstrip( field_buffer (field[1], 0) ), g_strstrip( field_buffer (field[2], 0) ),
malzeme_kodu_str, malzeme_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//faturakesilen tablosu
sprintf (query, "update faturakesilen set malzeme_kodu='%s', aciklama='%s' where malzeme_kodu='%s' and aciklama='%s'", 
g_strstrip( field_buffer (field[1], 0) ), g_strstrip( field_buffer (field[2], 0) ),
malzeme_kodu_str, malzeme_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//stokgelen tablosu
sprintf (query, "update stokgelen set malzeme_kodu='%s', aciklama='%s' where malzeme_kodu='%s' and aciklama='%s'", 
g_strstrip( field_buffer (field[1], 0) ), g_strstrip( field_buffer (field[2], 0) ),
malzeme_kodu_str, malzeme_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//stokgiden tablosu
sprintf (query, "update stokgiden set malzeme_kodu='%s', aciklama='%s' where malzeme_kodu='%s' and aciklama='%s'", 
g_strstrip( field_buffer (field[1], 0) ), g_strstrip( field_buffer (field[2], 0) ),
malzeme_kodu_str, malzeme_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//t�m tablolar�n guncellenmesi tmamaland�

			mesaj(duzenleme_basarili);
				
			for (i=0;i<9;i++);free_field(field[i]);
		
			stok_karton_eski(0);
			return;
			break;
			/*f2 kay�t i�lemlerinin sonu*/

		default:
			form_driver(my_form, ch);
			break;
	}

}

for (i=0;i<9;i++);free_field(field[i]);

}

/******************************************************************************/
