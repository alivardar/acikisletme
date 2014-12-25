
WINDOW *ana_win;



int market_ana_ekran()
{	

char *choices[] = {
    	_("Speed Sale with Barcode"),
    	_("Daily Sale Report"),
	_("Center Consolidation"),
	_("Back"),
        (char *)NULL,
          };


	ITEM **my_items;
	int c;				
	MENU *my_menu;
	WINDOW *my_menu_win;
	int n_choices, i;
	ITEM *cur_item;	

	initscr();
	start_color();

	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	ekran_temizle();

	ana_win = newwin(LINES-2, 80, 1, 0);
	touchwin(ana_win);
	wrefresh(ana_win);

	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	
	baslik_goruntule();
		
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
	print_in_middle(my_menu_win, 1, 0, 45, _("Kasa Satis Kayit"), COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 48);
	mvwaddch(my_menu_win, 2, 49, ACS_RTEE);
        
	post_menu(my_menu);
	wrefresh(my_menu_win);

	while((c = wgetch(my_menu_win)) != 274 ) 
	{       switch(c)
	        {	case KEY_DOWN:
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
						urun_satis();

        					touchwin(ana_win);
						wrefresh(ana_win);
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;

					case 2:	/* kasa durum raporu  */
//						urun_satis();
						mesaj( _("This side is not ready to use.") );

        					touchwin(ana_win);
						wrefresh(ana_win);
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;

					case 3:	/* Merkeze veri gonder  */
//						veri_gonder();
						mesaj( _("This side is not ready to use.") );
						
        					touchwin(ana_win);
						wrefresh(ana_win);
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						break;
						
					case 4:	/*çýkýþ*/

						touchwin(ana_win);
						wrefresh(ana_win);
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
		
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

//mysql_close(&mysql);

        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)  free_item(my_items[i]);

	return 0;
}



/******************************************************************************/

int urun_satis()
{

char query[2000]="";
char tarih[10]="";

char yazi[80];
	
char urun_adi[50];
char urun_bar_kod[20];
	
int miktar;
	
FIELD *field[9];
FORM  *my_form;
int ch,i,satir;
	
char birim_fiyat_str[100];
long int  birim_fiyat;	

long long int toplam=0;
char toplam_str[100];	
	
char miktarstr[20];
	
GList *ubarlist=NULL;
GList *urunlist=NULL;
GList *miklist=NULL;
GList *birimfiylist=NULL;

GList *l1;
GList *l2;
GList *l3;
GList *l4;

div_t bolme;

guint satir_sayisi;

touchwin(ana_win);
wrefresh(ana_win);

ekran_temizle();

//set_field_type(field[2], TYPE_ALNUM);
//set_field_type(field[6], TYPE_INTEGER);

/*row_no int*/	
/*Kayýt iþelminden sonra row no deðeri verilir ekrana yazýlýr*/
field[0] = new_field(1, 8, 2, 5, 0, 0);
/*tarih date*/	
field[1] = new_field(1, 2, 2, 24, 0, 0);
field[2] = new_field(1, 2, 2, 27, 0, 0);
field[3] = new_field(1, 4, 2, 30, 0, 0);
/*siparis_no char*/	
field[4] = new_field(1, 10, 2, 55, 0, 0);
/*kime bar kod char*/	
field[5] = new_field(1, 16, 3, 17, 0, 0);
//kime bilgisi
field[6] = new_field(1, 20, 3, 55, 0, 0);
//malzeme bar kod
field[7] = new_field(1, 16, 4, 17, 0, 0);
//satilan miktar
field[8] = new_field(1, 10, 4, 50, 0, 0);

//birim_fiyat  float
field[9] = NULL;

/*yazý yazýlamayan alanlar tanýmlandý*/
field_opts_off(field[0], O_ACTIVE);
field_opts_off(field[4], O_ACTIVE);
field_opts_off(field[6], O_ACTIVE);

/*miktar baslangýçta 1 olmalý*/
set_field_buffer(field[8], 0, "1" );

my_form = new_form(field);
post_form(my_form);

/*5nolu alan olan firma alnýndan baslamalý*/
//form_driver(field[5],REQ_FIRST_PAGE);
//3 kere next yapýnca firma barkod olur
form_driver(my_form, REQ_NEXT_FIELD);
form_driver(my_form, REQ_NEXT_FIELD);
form_driver(my_form, REQ_NEXT_FIELD);
form_driver(my_form, REQ_NEXT_FIELD);

attron(COLOR_PAIR(2));
mvprintw(1, 0, _("                          Speed Sales with Barcode                            ") );
baslik_goruntule();
attroff(COLOR_PAIR(2));	

mvprintw(2, 1, _("No:") );
mvprintw(2, 15, _("Date:") );
mvprintw(2, 26, ".");
mvprintw(2, 29, ".");
mvprintw(2, 40, _("Order No:") );

mvprintw(3, 1, _("Card No :") );
mvprintw(3, 40, _("Customer Name:") );
mvprintw(4, 1, _("Customer code:") );

mvprintw(4, 40, _("Amount:") );

attron(COLOR_PAIR(2));
mvprintw(5, 1, _("Material Barcode      ") );
mvprintw(5, 15, _("Name                        ") );
mvprintw(5, 47, _("Amount               ") );
mvprintw(5, 58, _("Unit Price           ") );
attroff(COLOR_PAIR(2));	

attron(COLOR_PAIR(1));
mvprintw(LINES - 3, 0, _(" F2->Save         ") );
mvprintw(LINES - 3, 20, _(" F3->New          ") );
mvprintw(LINES - 3, 40, _(" F5->Firm Select   ") );
mvprintw(LINES - 3, 60, _(" F6->Material Select") );
attroff(COLOR_PAIR(1));	

mvprintw(19, 0, "________________________________________________________________________________");
mvprintw(20, 30, _("Total  :") );

/*************************************************************/

/*row_no alaný ile ilgili deðer ekranda yazýlmasý gereken yere 
sql server içerisinden okunarak yazýlacaktýr*/

row_no_buyuk("stokgiden");
set_field_buffer(field[0], 0, rowno );

/*tarih içine degerler yazýlýyor*/
/*gun ay yil degerleri yaziliyor*/
tarih_ayarla();

set_field_buffer(field[1], 0,  gun );
set_field_buffer(field[2], 0,  ay );
set_field_buffer(field[3], 0,  yil );
/*************************************************************/

refresh();

while((ch = getch()) != 274  )
{
switch(ch)
{
case 8: /*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr konsolda 8 xterm key_backspace*/
	form_driver(my_form, REQ_PREV_CHAR);
	form_driver(my_form, REQ_DEL_CHAR);
	break;
case 9: /*TAB tusuna basýlmasý durumunda sonraki field konumlan*/
	form_driver(my_form, REQ_NEXT_FIELD);
	form_driver(my_form, REQ_END_LINE);
	break;		
case 10: /*ENTER tuþuna basýlmasý durumunda sonraki feild gececek*/
	/*Firma bilgisi bosmu kontrol edilecek  dolu ise ürün barkod kontrol edilecek */
		
	//5 barkodfirma 6 firma adi
	//7 malzeme cýktý alt satýra 8 miktar
		
form_driver(my_form, REQ_NEXT_FIELD);			
	//mesaj(field_buffer(field[5],0));
			
	/*eger firma girilmemsi ise*/			
/*
if ( (strlen(field_buffer(field[5],0))>10) && ( strlen( rtrim(field_buffer(field[6],0)," " ) ) <10 )  )
{
sprintf(query,"select firma_adi from firmalar where firma_bar_kod='%s' ", field_buffer(field[5],0) );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
set_field_buffer(field[6], 0, pgsql_kayit_field() );
//mesaj("girdi");	
form_driver(my_form, REQ_NEXT_FIELD);	
}
*/

		
//if  ( (strlen( rtrim(field_buffer(field[5],0)," ")  )>10) && (strlen(  rtrim( field_buffer(field[7],0)," ")   )>10) )
if  (strlen( rtrim( field_buffer(field[7],0)," ")   )>2) 
{
	sprintf(query,"select malzeme_adi,birim_fiyat from stokkart where seri_no='%s' ", field_buffer(field[7],0) );

	if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	strcpy(urun_adi, pgsql_kayit_field() );
	strcpy(birim_fiyat_str, pgsql_kayit_field() );
	
	if (strlen(urun_adi)<2)	 break;
										
	//butun degerleri deiþkenler icerisine sakla					
	strcpy( urun_bar_kod, field_buffer(field[7],0) );
	miktar=atoi( field_buffer(field[8],0) );
					
	sprintf(yazi, "%s - %s - %i - %s", urun_bar_kod, urun_adi, miktar, yazidan_paraya_cevir(birim_fiyat_str) );
					
	ubarlist=g_list_append(ubarlist, g_strdup(urun_bar_kod) );
	urunlist=g_list_append(urunlist, g_strdup(urun_adi) );
	sprintf(miktarstr, "%i",  miktar );
	miklist=g_list_append(miklist, g_strdup(miktarstr) );
	birimfiylist=g_slist_append(birimfiylist, g_strdup(birim_fiyat_str) );
						
	toplam=toplam+ (atoll(birim_fiyat_str) * atoll(miktarstr)  );
	sprintf(toplam_str,"%lli",toplam);
						
	mvprintw(20, 45, "%s", yazidan_paraya_cevir( toplam_str) );
						
	//Butun listeyi göster
			
	l1=g_list_nth(ubarlist, 0);

	satir_sayisi=g_list_length(l1);
	bolme=div(satir_sayisi,12);
	//var olan eskileri sil
	i=6;
	while (i<19)
	{			
	strcpy(yazi, "                                                                              ");
	mvprintw(i, 1, yazi);			
	i++;
	}
					
	l1=g_list_nth(ubarlist, bolme.quot*12);
	l2=g_list_nth(urunlist, bolme.quot*12);
	l3=g_list_nth(miklist, bolme.quot*12);
	l4=g_list_nth(birimfiylist, bolme.quot*12);
					
	i=6;
	while (l1!=NULL)
	    {			
	    sprintf(yazi, "%16.16s %28.28s %4.4s %27.27s", (gchar *)l1->data, (gchar *)l2->data,
	    (gchar *)l3->data, yazidan_paraya_cevir((gchar *)l4->data)  );
	    mvprintw(i, 1, yazi);
	    i++;
	    l1=l1->next;
	    l2=l2->next;
	    l3=l3->next;
	    l4=l4->next;
	    }
	refresh;
					
	/**************/
	}// bu alan sql okuma sonrasýdýr
				
	//7 bosalt miktar 1 yap
	set_field_buffer(field[7], 0, "" );
	set_field_buffer(field[8], 0, "1" );
	//1 geri gel ki tekrar barkod okumasý yapýlsýn
	form_driver(my_form, REQ_PREV_FIELD);
	refresh;
			
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
case KEY_F(5):/*kimden seçim menüsü*/
	secilen=field_goster("firmalar", "firma_adi", 0);
	set_field_buffer(field[6], 0,  secilen);
	/*aþaðýdaileri yapýnca refresh yapýyor*/
	form_driver(my_form, REQ_NEXT_FIELD);
	form_driver(my_form, REQ_END_LINE);
	break;
case KEY_F(6):/*stok no menüsü*/		
	secilen=field_goster("stokkart", "seri_no", 0);		
	set_field_buffer(field[7], 0,  secilen);
	/*aþaðýdaileri yapýnca refresh yapýyor*/
	form_driver(my_form, REQ_NEXT_FIELD);
	form_driver(my_form, REQ_END_LINE);
	break;
case KEY_F (1):
	  tus_yardim();
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  refresh();
	  break;	
case KEY_F(2):/*kayýt etme iþlemi*/	
	/*sql yýl ay gun alýyor*/
	sprintf(tarih,"%s.%s.%s", field_buffer(field[1],0), 
	field_buffer(field[2],0), field_buffer(field[3],0) );
			
	l1=g_list_nth(ubarlist,0);
	l2=g_list_nth(urunlist,0);
	l3=g_list_nth(miklist,0);
	l4=g_list_nth(birimfiylist,0);
	i=0;	

	while (l1!=NULL)
{			
/*
sprintf(yazi, " %-16s %-28s %6s %24s ", (gchar *)l1->data, (gchar *)l2->data,
(gchar *)l3->data, yazidan_paraya_cevir((gchar *)l4->data)  );
*/
sprintf(query, 
"insert into stokgiden (tarih, malzeme_kodu, belge_no, aciklama, birim_fiyat, giden, kime ) values (current_date,'%s','%s','%s', '%s', '%s','%s' );",
/*malzeme_bar_kod, sipariþ no, aciklama(urun_adi)*/
(gchar *)l1->data, "Perakende", (gchar *)l2->data,
/*birim_fiyat, adet */
(gchar *)l4->data, (gchar *)l3->data,
/*kime*/
"Perakende"  );


if (pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

l1=l1->next;
l2=l2->next;
l3=l3->next;
l4=l4->next;
i++;//row_no alaný artýrýlacak

//tum listenin dongu sonu	
}
		
/*stok kayýdý yapýldýktan sonra kullanýcý girmek isteyebilir o sorulacak ve 
sonrasýnda field alanlarý bosaltýlacak*/
	
donen_deger=onayla( _("Kayýt baþarý ile gerçekleþtirildi. Yeni kayýt girecekmisiniz?") );
if (donen_deger==1)
{
/*evet deðeri girildi*/				
/*row_no alaný ogrenilerek 1 artýlýp ekrana yazýlacaktýr*/
row_no_buyuk("stokgiden");
set_field_buffer(field[0], 0, rowno );
			
/*tarih içine degerler yazýlýyor*/
/*gun ay yil degerleri yaziliyor*/
tarih_ayarla();

set_field_buffer(field[1], 0,  gun );
set_field_buffer(field[2], 0,  ay );
set_field_buffer(field[3], 0,  yil );
				
/*diger alanlar ise bosaltýlacaktýr*/
set_field_buffer(field[4], 0, "" );
set_field_buffer(field[5], 0, "" );
set_field_buffer(field[6], 0, "" );
				
/*ayrýca ekranýn altýna yazýlan satýlan mal alanlarýda silinecek*/
//listeyie ekrandan sil
i=6;
while (i<21)
{			
strcpy(yazi, "                                                                              ");
mvprintw(i, 1, yazi);						
i++;
}
mvprintw(19, 0, "________________________________________________________________________________");
mvprintw(20, 30, "Total :");
refresh;
					
}
if (donen_deger==0)
{
return;
}
break;
/*f2 kayýt iþlemlerinin sonu*/

case KEY_F(3):/*gelen stok yeni kayýt*/
/*f3 iþleminin baþý*/
donen_deger=onayla( _("All sales (in screen) will be delete. Are you sure.?") );
if (donen_deger==1)
    {
    /*evet deðeri girildi*/				
    /*row_no alaný ogrenilerek 1 artýlýp ekrana yazýlacaktýr*/
    row_no_buyuk("stokgelen");
    set_field_buffer(field[0], 0, rowno );
    /*tarih içine degerler yazýlýyor*/
    /*gun ay yil degerleri yaziliyor*/
    tarih_ayarla();

    set_field_buffer(field[1], 0,  gun );
    set_field_buffer(field[2], 0,  ay );
    set_field_buffer(field[3], 0,  yil );
				
    /*diger alanlar ise bosaltýlacaktýr*/
    set_field_buffer(field[4], 0, "" );
    set_field_buffer(field[5], 0, "" );
    set_field_buffer(field[6], 0, "" );
    set_field_buffer(field[7], 0, "" );
    set_field_buffer(field[8], 0, "" );
    set_field_buffer(field[9], 0, "" );
    }
break;

default:
form_driver(my_form, ch);
break;
}

}

//unpost_form(my_form);
//free_form(my_form);

}


/******************************************************************************/
