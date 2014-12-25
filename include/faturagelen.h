

char *faturasi_gelmemis_irsaliye_goster (unsigned long int kayit_no_gelen)
{
char query[2000] = "";
char gecici_yazi[1000] = "";
FIELD *field[7];
FORM *my_form;
int ch, i, n_choices, c;

ITEM *my_items[20];
ITEM *cur_item;
MENU *my_menu;
WINDOW *my_menu_win;

char *deger[20][60];
char *deger2[20][60];

char donen[60];
int *donen_deger;
char *yazi;

  touchwin (ana_win);
  wrefresh (ana_win);

  row_no_buyuk ("irsaliyeler");
  if (row_no < kayit_no_gelen) kayit_no_gelen = kayit_no_gelen - 15;

  sprintf (query,
  "select distinct irsaliye_no, tarih, firma_adi from irsaliyeler where fatura_no='' and tur='0' order by firma_adi limit 15 offset %li",
  kayit_no_gelen);

  if (strlen (g_strstrip(giris)) > 2)
sprintf (query,
"select irsaliye_no, tarih, firma_adi from irsaliyeler where kimden like '%%%s%%' and fatura_no='' and tur='0' order by firma_adi limit 15 offset 0",
giris);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
  
    for (i=0;i<pgsql_row_miktar;i++)
    {
      sprintf (deger[i], "%-10s", pgsql_kayit_field() );
		sql_tarih_ayir(pgsql_kayit_field());
      sprintf (deger2[i], "%2.2s/%2.2s/%4.4s  %-25.25s", sql_gun, sql_ay, sql_yil, pgsql_kayit_field() );	  
	  my_items[i] = new_item (&deger[i], &deger2[i]);  
    }

  my_items[i] = (ITEM *) NULL;
  my_menu = new_menu ((ITEM **) my_items);
  /*3 satýr 3 sutun 15 satýr 60 sutun pencere */
  my_menu_win = newwin (20, 75, 3, 3);
  keypad (my_menu_win, TRUE);
  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 15, 70, 4, 1));
  set_menu_format (my_menu, 15, 1);
  set_menu_mark (my_menu, mark);
  box (my_menu_win, 0, 0);
  print_in_middle (my_menu_win, 1, 0, 75,   _("   F5-> Ara    ESC-> Çýkýþ   Seçiminizi Yapýnýz "), COLOR_PAIR (1));
  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, 80);
  mvwaddch (my_menu_win, 2, 79, ACS_RTEE);

  post_menu (my_menu);
  wrefresh (my_menu_win);

  menu_driver (my_menu, REQ_DOWN_ITEM);
  menu_driver (my_menu, REQ_UP_ITEM);

  while ((c = getch ()) != 10)
    {
      switch (c)
	{
	case KEY_DOWN:
	  menu_driver (my_menu, REQ_DOWN_ITEM);
	  break;
	case KEY_UP:
	  menu_driver (my_menu, REQ_UP_ITEM);
	  break;
	case KEY_NPAGE:
	  kayit_no_gelen = kayit_no_gelen + 15;
	  sprintf (donen, "%s", item_name (cur_item));

	  kesilmemis_irsaliye_goster (kayit_no_gelen);
	  return donen;

	case KEY_PPAGE:
	  if (kayit_no_gelen > 14){kayit_no_gelen = kayit_no_gelen - 15; }
	  cur_item = current_item (my_menu);
	  sprintf (donen, "%s", kesilmemis_irsaliye_goster (kayit_no_gelen));
	  return donen;

	case KEY_F (5):
	  aranacak_kelime ( _("Ürün adý veya içinde geçen bir deðer giriniz.    Düzgün sýralama için boþ býrakýnýz. ") );
	  sprintf (donen, "%s", kesilmemis_irsaliye_goster (kayit_no_gelen));
	  return donen;

	}
      cur_item = current_item (my_menu);
      wrefresh (my_menu_win);
    }

  cur_item = current_item (my_menu);
  sprintf (donen, "%s", item_name (cur_item));

  return donen;
}


/*********************************************************/

int fatura_gelen_islemleri ()
{
char query[2000] = "";
char tarih[10] = "";

char durum = 0;		//0=gezinti, 1=yeni, 2=duzeltme

FIELD *field[118];
FORM *my_form;
int ch, satir, sira;

long int rowno_miktar;
long int rownno;

long long int toplam;
long long int toplam_kdv;
long long int toplam_tutar;
long long int tutar;
long long int isk_tutar;
long long int isk_toplam_tutar;
long long int genel_toplam;
long long int yekun;

char isk_oran_str[5];
char yekun_str[30];
char isk_toplam_tutar_str[30];
char toplam_tutar_str[30];
char tutar_str[30];
char toplam_str[30];
char toplam_kdv_str[30];
char genel_toplam_str[30];

char firma_adi_str[100];	
unsigned long long int birim_fiyat;
int kdv_oran;
int miktar;

long long int kdv_tutar;
char kdv_tutar_str[30];

long long int kdv_tutar_kayit;
char kdv_tutar_kayit_str[30];

long long int _birim_fiyat;
int _miktar;
int _kdv_oran;

char deger[50];

unsigned int max_fatura_no;
char max_fatura_no_str[20];

touchwin(ana_win);
wrefresh(ana_win);

ekran_temizle();

/*row_no int*/
/*Kayýt iþleminden sonra row no deðeri verilir ekrana yazýlýr*/
  field[0] = new_field (1, 8, 1, 5, 0, 0);
/*tarih date*/
  field[1] = new_field (1, 2, 1, 22, 0, 0);
  field[2] = new_field (1, 2, 1, 25, 0, 0);
  field[3] = new_field (1, 4, 1, 28, 0, 0);
/*belge_no char fatura*/
  field[4] = new_field (1, 12, 1, 42, 0, 0);

/*irsaliye no*/
  field[5] = new_field (1, 15, 1, 64, 0, 0);

/*kime char*/
  field[6] = new_field (1, 70, 2, 10, 0, 0);

//alt satýrdaki toplam kdv ve yekun için alanlar
//toplam
  field[7] = new_field (1, 25, 20, 53, 0, 0);
//kdv toplam
  field[8] = new_field (1, 25, 21, 53, 0, 0);
//yekun
  field[9] = new_field (1, 25, 22, 53, 0, 0);
	
//alt satýrdaki toplam kdv ve yekun için alanlar
//toplam
  field[10] = new_field (1, 20, 20, 18, 0, 0);
//kdv toplam
  field[11] = new_field (1, 20, 21, 18, 0, 0);
//yekun
  field[12] = new_field (1, 20, 22, 18, 0, 0);

field_opts_off (field[0], O_ACTIVE);
		
satir = 4;
for (i = 13; i < 115; i = i + 7)
    {
	//malzeme_kodu char
      field[i] = new_field (1, 16, satir, 1, 1, 0);
	//urun adý
      field[i + 1] = new_field (1, 14, satir, 18, 2, 0);
	//miktar
      field[i + 2] = new_field (1, 6, satir, 33, 1, 0);
	//birim
      field[i + 3] = new_field (1, 5, satir, 40, 1, 0);
	//kdv
      field[i + 4] = new_field (1, 3, satir, 46, 0, 0);
	//birim_fiyat
      field[i + 5] = new_field (1, 12, satir, 50, 1, 0);
	//tutar
      field[i + 6] = new_field (1, 15, satir, 64, 2, 0);
	  
	  set_field_type(field[i + 2], TYPE_INTEGER, 0);
	  set_field_just (field[i + 2], JUSTIFY_RIGHT);
		  
	  set_field_type(field[i + 3], TYPE_INTEGER, 0);
	  set_field_just (field[i + 3], JUSTIFY_RIGHT);	

	  set_field_type(field[i + 4], TYPE_INTEGER, 0);
	  set_field_just (field[i + 4], JUSTIFY_RIGHT);
		
	  set_field_type(field[i + 5], TYPE_ALNUM, 0);
	  set_field_just (field[i + 5], JUSTIFY_RIGHT);
	  	  
	  set_field_type(field[i + 6], TYPE_ALNUM, 0);
	  set_field_just (field[i + 6], JUSTIFY_RIGHT);

	  satir = satir + 1;
    }

  field[118] = NULL;
  
  my_form = new_form (field);
  post_form (my_form);

refresh (); 

//field tipleri belirleniyor
  set_field_just (field[4], JUSTIFY_LEFT);
  set_field_just (field[5], JUSTIFY_LEFT);
  set_field_just (field[6], JUSTIFY_LEFT);
//tarih alanýna int girmek zorundalar
	set_field_type (field[1], TYPE_INTEGER);
	set_field_type (field[2], TYPE_INTEGER);
	set_field_type (field[3], TYPE_INTEGER);

	set_field_type (field[7], TYPE_ALNUM);
	set_field_type (field[8], TYPE_ALNUM);
	set_field_type (field[9], TYPE_ALNUM);

	set_field_type (field[10], TYPE_ALNUM);
	set_field_type (field[11], TYPE_INTEGER);
	set_field_type (field[12], TYPE_ALNUM);

	set_field_just (field[7], JUSTIFY_RIGHT );
	set_field_just (field[8], JUSTIFY_RIGHT);
	set_field_just (field[9], JUSTIFY_RIGHT);

	set_field_just (field[10], JUSTIFY_RIGHT );
	set_field_just (field[11], JUSTIFY_RIGHT);
	set_field_just (field[12], JUSTIFY_RIGHT);


for (i=7;i<13;i++)
{
	set_field_fore(field[i], COLOR_PAIR(2) );
	set_field_back(field[i], COLOR_PAIR(2) );
}

for (i = 0; i < 118; i++)
{
    field_opts_off (field[i], O_EDIT);
	field_opts_off (field[i], O_AUTOSKIP);
}

field_opts_off (field[7], O_ACTIVE);
field_opts_off (field[8], O_ACTIVE);
field_opts_off (field[9], O_ACTIVE);
field_opts_off (field[10], O_ACTIVE);
field_opts_off (field[12], O_ACTIVE);

attron (COLOR_PAIR (2));
attron(A_BOLD);
mvprintw (0, 0,  _("                      Acikiþletme Gelen Fatura Ekraný                          ") );
attroff(A_BOLD);
attroff (COLOR_PAIR (2));

/*toplam 3 satýr*/
  mvprintw (1, 1,  _("No:") );
  mvprintw (1, 15, _("Tarih:") );
  mvprintw (1, 24, "/");
  mvprintw (1, 27, "/");
  mvprintw (1, 34, _("Fat.No:") );

  mvprintw (1, 55, _("Ýrs.No:") );
  mvprintw (2, 1,  _("Kimden:") );

//kalýn yazmaya baþla
attron(A_BOLD);

//baslýk
attron (COLOR_PAIR (2));
  mvprintw (3, 0,  _(" Malzeme Kodu           ") );
  mvprintw (3, 17, _(" Ürünün Adý            ") );
  mvprintw (3, 32, _(" Miktar") );
  mvprintw (3, 39, _(" Birim") );
  mvprintw (3, 45, _(" KDV ") );
  mvprintw (3, 49, _(" Birim Fiyatý   ") );
  mvprintw (3, 63, _(" Tutarý          ") );
  attroff (COLOR_PAIR (2));
  
attroff(A_BOLD);  
  
  //cizgi cizliecek
  for (i=4;i<19;i++) 
  {
	  mvaddch (i, 0, ACS_VLINE);
	  mvaddch (i, 17, ACS_VLINE);
	  mvaddch (i, 32, ACS_VLINE);
	  mvaddch (i, 39, ACS_VLINE);
	  mvaddch (i, 45, ACS_VLINE);
	  mvaddch (i, 49, ACS_VLINE);
	  mvaddch (i, 63, ACS_VLINE);
	  mvaddch (i, 79, ACS_VLINE);
  }

//19 nolu satýrda tüm pencerenin alt kýsmý tamamlanýyor
mvaddch (19, 0, ACS_LTEE);

for (i=1;i<17;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 17, ACS_BTEE);

for (i=18;i<32;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 32, ACS_BTEE);

for (i=33;i<39;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 39, ACS_PLUS);

for (i=40;i<45;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 45, ACS_BTEE);

for (i=46;i<49;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 49, ACS_BTEE);

for (i=50;i<63;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 63, ACS_BTEE);

for (i=64;i<79;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 79, ACS_RTEE);
//19 satýr i,þlemleri tamamlandý

//23 satýr iþlemleri
mvaddch (23, 0, ACS_LLCORNER);

for (i=1;i<39;i++) mvaddch (23, i, ACS_HLINE);
mvaddch (23, 39, ACS_BTEE);

for (i=40;i<79;i++) mvaddch (23, i, ACS_HLINE);
mvaddch (23, 79, ACS_LRCORNER);
//23 satur sonu 


for (i=20;i<23;i++)
{
mvaddch (i, 0, ACS_VLINE);
mvaddch (i, 39, ACS_VLINE);
mvaddch (i, 79, ACS_VLINE);
}

attron(A_BOLD);
mvprintw (20, 1, _("Ara toplam     :") );
mvprintw (21, 1, _("Ýskonto oran(%):") );   
mvprintw (22, 1, _("Ýskonto tutarý :") );

mvprintw (20, 40, _("Genel toplam:") );
mvprintw (21, 40, _("KDV         :") );  
mvprintw (22, 40, _("Yekün       :") );
attroff(A_BOLD);
  
mvprintw (24 , 0, _("   F1->Yardým   F2->Kaydet  F3->Yeni   F4->Sil   F8->Menü   Gezinti PgUp/PgDn") );
  

/*************************************************************/
//ilk acýlýnca en son kayýt uzerine konumlan

  row_no_buyuk_tur ("faturalar", "0");
  sprintf (query,
  "select row_no, tarih, fatura_no,irsaliye_no, firma_adi, toplam, kdv, toplam_tutar, isk_oran, isk_tutar from faturalar where row_no=%i and tur='0' ",
  row_no - 1);
  
  pgsql_sorgula(query);
  
//row no
  set_field_buffer (field[0], 0, pgsql_kayit_field() );
//tarih
  sql_tarih_ayir ( pgsql_kayit_field() );
  set_field_buffer (field[1], 0, sql_gun);
  set_field_buffer (field[2], 0, sql_ay);
  set_field_buffer (field[3], 0, sql_yil);
//fatura ve irsaliye no
  set_field_buffer (field[4], 0, pgsql_kayit_field() );
  set_field_buffer (field[5], 0, pgsql_kayit_field() );
//firma adi
  set_field_buffer (field[6], 0, pgsql_kayit_field() );
//toplam
  set_field_buffer (field[7], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//kdv
  set_field_buffer (field[8], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//toplam_tutar
  set_field_buffer (field[9], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//isk_oran
  set_field_buffer (field[11], 0, pgsql_kayit_field() );
//isk_tutar
  set_field_buffer (field[12], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//ara toplam
  sprintf(toplam_str,"%lli", atoll(paradan_yaziya_cevir(field_buffer(field[7],0)))+
	atoll( paradan_yaziya_cevir(field_buffer(field[12],0) )  ) );
  set_field_buffer (field[10], 0, yazidan_paraya_cevir(toplam_str) );
 
  sprintf (query,
  "select malzeme_kodu, aciklama, miktar, birim_tip, kdv_oran, birim_fiyat,tutar from faturagelen where tarih='%s-%s-%s'  and belge_no='%s' and irsaliye_no='%s' ",
  g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
  g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[4], 0)),
  g_strstrip(field_buffer (field[5], 0))  );

//burada kontrol yapma cunku ilk kayit olabilir
  pgsql_sorgula(query);
  
  i = 13;
  for (j=0;j<pgsql_row_miktar;j++)
    {
	set_field_buffer (field[i], 0, pgsql_kayit_field() );
	set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
	set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
	set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
	set_field_buffer (field[i + 4], 0, pgsql_kayit_field() );
	set_field_buffer (field[i + 5], 0, yazidan_paraya_cevir (pgsql_kayit_field()) );
      	set_field_buffer (field[i + 6], 0, yazidan_paraya_cevir (pgsql_kayit_field()) );
      i = i + 7;
    }

	
/*************************************************************/

  refresh ();

  while ((ch != 27))
    {
      ch = getch ();
      switch (ch)
	{
	
	case KEY_F (10):
	  ch = 27;
	  break;

	case KEY_F (1):
	  tus_yardim();
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  refresh();
	  break;

	case KEY_BACKSPACE: /*konsolda 8 xterm key_backspace */
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;

	case 8:		/*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr konsolda 8 xterm key_backspace */
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case 9:		/*TAB tusuna basýlmasý durumunda bir onceki field konumlan */
	  form_driver (my_form, REQ_PREV_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;

	case 10:	/*ENTER tuþuna basýlmasý durumunda sonraki feild gececek */
//otomatik hesaplamalr yapýlacak
		//Butun satýrlarýn tutar hanesi yazýlacak
	if (durum==1){

		form_driver (my_form, REQ_NEXT_FIELD);
		form_driver (my_form, REQ_PREV_FIELD);

		tutar=0;
		toplam_tutar=0;
		toplam_kdv=0;
		isk_toplam_tutar=0;
		strcpy(toplam_tutar_str,"");
		strcpy(toplam_kdv_str,"");
		strcpy(genel_toplam_str,"");

		for (i=13;i<118;i=i+7)
		{
		tutar=atoi(field_buffer(field[i+2],0))*atoll(field_buffer(field[i+5],0));
		if (tutar>0){sprintf(tutar_str,"%lli",tutar);set_field_buffer (field[i + 6], 0, tutar_str);}
		}

		//butun satýrlarýn tutar toplamý alýnýr kdv toplamý alýnýr
		for (i=13;i<118;i=i+7)
		{
		toplam_tutar=toplam_tutar+atoll(field_buffer(field[i+6],0));
		isk_tutar=( atoll(field_buffer(field[i+6],0)) * atoi( field_buffer(field[11],0) ) /100 );
		isk_toplam_tutar=isk_toplam_tutar+isk_tutar;
		toplam_kdv=toplam_kdv+((atoll(field_buffer(field[i+6],0))-isk_tutar)*atoi(field_buffer(field[i+4],0))/100);
		}
		
		genel_toplam=toplam_tutar-isk_toplam_tutar;
		yekun=genel_toplam+toplam_kdv;
		
		sprintf(toplam_tutar_str, "%lli", toplam_tutar);
		sprintf(toplam_kdv_str, "%lli", toplam_kdv);		
		sprintf(isk_toplam_tutar_str, "%lli", isk_toplam_tutar);
		sprintf(genel_toplam_str,"%lli", genel_toplam);		
		sprintf(yekun_str, "%lli", yekun);

		//ara toplam yazýldý
		set_field_buffer (field[10], 0, yazidan_paraya_cevir(toplam_tutar_str));
		//iskonto toplamý
		set_field_buffer (field[12], 0, yazidan_paraya_cevir(isk_toplam_tutar_str));
		
		//genel toplam
		set_field_buffer (field[7], 0, yazidan_paraya_cevir(genel_toplam_str));
		//kdv toplamý
		set_field_buffer (field[8], 0, yazidan_paraya_cevir(toplam_kdv_str));
		//yekun
		set_field_buffer (field[9], 0, yazidan_paraya_cevir(yekun_str));

	}//durum 1 ise

	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;

	case KEY_DC:		/*del tusu */	
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case KEY_HOME:
	  form_driver (my_form, REQ_BEG_LINE);
	  break;
	case KEY_END:
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_DOWN:
	if ((field_index(current_field(my_form)) +7) <118) 
		set_current_field(my_form, field[field_index(current_field(my_form)) +7] );
	  break;
	case KEY_UP:
	if ((field_index(current_field(my_form)) -7) >1) 
		set_current_field(my_form, field[field_index(current_field(my_form)) -7] );
	  break;
	case KEY_LEFT:
	//yeni kayýt ise
	  if (durum == 1) { form_driver (my_form, REQ_LEFT_CHAR);break;}
	  form_driver (my_form, REQ_PREV_FIELD);
	  form_driver (my_form, REQ_END_LINE);				
	  break;
	case KEY_RIGHT:
	  //yeni kayýt ise
	  if (durum == 1){ form_driver (my_form, REQ_RIGHT_CHAR); break;}
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);		
	  break;


	case KEY_F (8):	/*seçenek menüsü */
		
//öncelikle yeni mi duzeltmemi bakýlacak
if ((durum == 1) || (durum == 2))	//1=yeni ise , 1=duzeltme ise sadece
   {
	//firma secilmek istenirse
	if ( field_index(current_field(my_form)) == 6 ) 
		{
		secilen = field_goster ("firmalar", "firma_adi", 0);
        set_field_buffer (field[6], 0, secilen);
        /*aþaðýdakileri yapýnca refresh yapýyor */
        form_driver (my_form, REQ_NEXT_FIELD);
		form_driver (my_form, REQ_END_LINE);
		break;
		}

	bolme= div(field_index(current_field(my_form))-13, 7);

	//ürün secilmek istenirse
	if ( bolme.rem == 0) 
		{			
         form_driver (my_form, REQ_NEXT_FIELD);
         form_driver (my_form, REQ_PREV_FIELD);
         secilen = field_goster ("stokkart", "malzeme_adi", 0);
         i = 13;
        while (i < 118)
			{
			  sprintf (query, "%s",
			   rtrim (field_buffer (field[i], 0), " "));
				  if (strlen (query) < 1)
				{
				  set_field_buffer (field[i + 1], 0, secilen);
				  //field icindeki deger sql icinden okunacak ve sonaki field 
				  //icine yazýlacak urun adý
				  sprintf (query,
				   "select malzeme_kodu,malzeme_adi, birim_tip, kdv_orani, birim_fiyat from stokkart where malzeme_adi like '%%%s%%' ",
				  g_strstrip (field_buffer (field[i + 1], 0)));
				
				  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
				  set_field_buffer (field[i], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+1], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+3], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+4], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+5], 0, pgsql_kayit_field() );
				  
				  //konumlan                                      
				  form_driver (my_form, REQ_NEXT_FIELD);
				  form_driver (my_form, REQ_PREV_FIELD);
				  break;
					}
			  i = i + 7;
			}

		  form_driver (my_form, REQ_NEXT_FIELD);
		  form_driver (my_form, REQ_PREV_FIELD);
		}

		//faturasý gelmemis irsaliye secilmek istenirse
		if ( field_index(current_field(my_form)) == 5) 
			{
			  set_field_buffer (field[5], 0, faturasi_gelmemis_irsaliye_goster (0) );
			  /*aþaðýdakileri yapýnca refresh yapýyor */
			  form_driver (my_form, REQ_NEXT_FIELD);
			  form_driver (my_form, REQ_PREV_FIELD);

			  sprintf (query,
"select malzeme_kodu, aciklama, gelen, birim_tip, birim_fiyat, kimden from stokgelen where belge_no='%s'  ",
			  g_strstrip(field_buffer (field[5], 0)), g_strstrip(field_buffer (field[5], 0)) );
	      
			  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
			  i = 13;
			  for (j=0;j<pgsql_row_miktar;j++)
				  {
				  set_field_buffer (field[i], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i + 5], 0, pgsql_kayit_field() );
				  //firma adý
				  set_field_buffer (field[6], 0, pgsql_kayit_field() );				  
				  i = i + 7;
				  }

			//irsaliye satýrlarý yazýldýktan sonra kdv roanlarý yazýlacak
			  for (i=13;i<118;i=i+7)
				{
			  sprintf (query, "select kdv_orani from stokkart where malzeme_kodu='%s'  ",
													 g_strstrip(field_buffer (field[i], 0))  );
	      	  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			  set_field_buffer (field[i+4], 0, pgsql_kayit_field() );
				}

				  /*aþaðýdakileri yapýnca refresh yapýyor */
				  form_driver (my_form, REQ_NEXT_FIELD);
				  form_driver (my_form, REQ_PREV_FIELD);;
		
			}

		}
		break;
	case KEY_F (2):  //kaydetme iþlemleri
	  	toplam = 0;
		if (durum == 1)	//sadece yeni kayýt ise calýs
	    	{
	      	form_driver (my_form, REQ_NEXT_FIELD);
	      	form_driver (my_form, REQ_PREV_FIELD);
	      	if (strlen (rtrim (field_buffer (field[4], 0), " ")) < 2)
		{
		mesaj ("Fatura no. yazmadýnýz.");
		form_driver (my_form, REQ_NEXT_FIELD);
	      	form_driver (my_form, REQ_PREV_FIELD);
		break;
		}

	      if (strlen (rtrim (field_buffer (field[6], 0), " ")) < 2)
		{
		mesaj ("Firma adý girmek zorundasýnýz.");
		form_driver (my_form, REQ_NEXT_FIELD);
	      	form_driver (my_form, REQ_PREV_FIELD);
		break;
		}

	      if (strlen (rtrim (field_buffer (field[7], 0), " ")) < 2)
		{
		mesaj ("Kayýt girmediniz.");
		form_driver (my_form, REQ_NEXT_FIELD);
	        form_driver (my_form, REQ_PREV_FIELD);
		break;
		}

	      	sprintf (tarih, "%s.%s.%s", g_strstrip(field_buffer (field[3], 0)),
		g_strstrip(field_buffer (field[2], 0)), g_strstrip(field_buffer (field[1],0))  );

	      	rowno_miktar = atoi (field_buffer (field[0], 0));
	      	for (i = 13; i < 118; i = i + 7)
		{
		//kontrol et yoksa glib null dondu hata veriyor
		if ((strlen (rtrim(field_buffer (field[i], 0), " ")) > 2) && (atoi (field_buffer (field[i + 2], 0)) > 0))
		    {
		sprintf (query,
"insert into faturagelen (tarih, malzeme_kodu, belge_no, irsaliye_no, aciklama, birim_fiyat, miktar, kimden, kdv_oran, kdv_tutar, birim_tip, tutar) values ('%s','%s','%s','%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
			   /*row_no, tarih */
			    tarih,
			   /*malzeme_kodu, belge no, irsaliye_no */
				g_strstrip(field_buffer (field[i], 0)),
				g_strstrip(field_buffer (field[4], 0)),
				g_strstrip(field_buffer (field[5], 0)),
			   /*urun adi, birim_fiyat */
				g_strstrip(field_buffer (field[i + 1], 0)),
				g_strstrip(field_buffer (field[i + 5], 0)),
			   /*miktar, kimden */
				g_strstrip(field_buffer (field[i + 2], 0)),
				g_strstrip(field_buffer (field[6], 0)),
				g_strstrip(field_buffer (field[i + 4], 0)),
				//kdv_tutar_kayit_str ,
				"",
				g_strstrip(field_buffer (field[i + 3], 0)),
				g_strstrip(field_buffer (field[i + 6], 0))
				
				);

				if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			}

		}

		sprintf(toplam_str, "%s", paradan_yaziya_cevir( g_strstrip(field_buffer (field[7], 0)) ) );
		sprintf(kdv_tutar_str, "%s", paradan_yaziya_cevir( g_strstrip(field_buffer (field[8], 0)) ) );
		sprintf(genel_toplam_str, "%s", paradan_yaziya_cevir( g_strstrip(field_buffer (field[9], 0)) ) );
		sprintf(isk_oran_str, "%s", paradan_yaziya_cevir( g_strstrip(field_buffer (field[11], 0)) ) );
		sprintf(isk_toplam_tutar_str, "%s", paradan_yaziya_cevir( g_strstrip(field_buffer (field[12], 0)) ) );

	      //kayýt iþlemleri bittikten sonra ilgili irsaliye alanýna fatura numarasý iþlenmlidir
	      sprintf (query,
		  "insert into faturalar (tarih, fatura_no, irsaliye_no, firma_adi, toplam, kdv, toplam_tutar, tur, isk_oran, isk_tutar) values ('%s','%s','%s','%s','%s', '%s', '%s', '%s', '%s', '%s')",
		  //tarih
		  tarih,
		  //fatura_no, irsaliye_no
		  g_strstrip(field_buffer (field[4], 0)), g_strstrip(field_buffer (field[5], 0)),
		  //firma_adi ,toplam
		  g_strstrip(field_buffer (field[6], 0)),
		 toplam_str, kdv_tutar_str, genel_toplam_str,
		  //tur 0 ise gelen fatura,  1 ise kesilen fatura
		  "0",
			isk_oran_str,
			isk_toplam_tutar_str
		   );

		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

		  //firmanýn borcuna iþlendi
		cari_miktar_isle ( g_strstrip(field_buffer (field[6], 0)), atoll (toplam_tutar_str), -1);
		    
		
		//stokgelen tablosu kayýt iþlemleri bittikten sonra ilgili irsaliye alanýna fatura numarasý iþlenmlidir
	      sprintf (query,
          "update stokgelen set fatura_no='%s' where (belge_no='%s') ",
	      g_strstrip(field_buffer (field[4], 0)), g_strstrip(field_buffer (field[5], 0))  );
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

		//irsaliyeler tablosu kayýt iþlemleri bittikten sonra ilgili irsaliye alanýna fatura numarasý iþlenmlidir
	      sprintf (query,
          "update irsaliyeler set fatura_no='%s' where (irsaliye_no='%s') ",
	      g_strstrip(field_buffer (field[4], 0)), g_strstrip(field_buffer (field[5], 0))  );
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

		
	  /*stok kayýdý yapýldýktan sonra kullanýcý girmek isteyebilir o sorulacak ve 
	     sonrasýnda field alanlarý bosaltýlacak */

	  donen_deger = onayla (yeni_kayit_istiyormusunuz);
	  if (donen_deger == 0) return;
	  if (donen_deger == 1)
	    {
	      /*evet deðeri girildi */
	      /*row_no alaný ogrenilerek 1 artýlýp ekrana yazýlacaktýr */
	      row_no_buyuk ("faturagelen");
	      set_field_buffer (field[0], 0, rowno);

	      /*tarih içine degerler yazýlýyor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

	      for (i = 4; i < 118; i++)  set_field_buffer (field[i], 0, "");
		  // fatura alanýna konumlan
		  set_current_field(my_form, field[4] );
	    }

	  /*f2 kayýt iþlemlerinin sonu */
}

	  //durum sonu
	  //yeni ise kaydet daha sonra duzeltmeye gore duzenlenecke                                       
	  break;


	case KEY_F (3):	/*yeni kayýt */
		donen_deger = onayla (yeni_kayit);
		if (donen_deger == 1)
	    	{
	      	/*evet deðeri girildi */
	      	/*row_no alaný ogrenilerek 1 artýlýp ekrana yazýlacaktýr */
	      	row_no_buyuk ("faturagelen");
	      	set_field_buffer (field[0], 0, rowno);
		for (i = 1; i < 118; i++)
		{  field_opts_on (field[i], O_EDIT);
		    set_field_buffer (field[i], 0, ""); }

	        //firma adý alaný yazýlmaz olmalý
		field_opts_off (field[6], O_EDIT);
		//toplam yekunler icin alanlar
		field_opts_off (field[7], O_EDIT);
		field_opts_off (field[8], O_EDIT);
		field_opts_off (field[9], O_EDIT);	      

		//fatura no en son olana bir ekleme yapýlarak yazýlacak

		/*tarih içine degerler yazýlýyor */
	        /*gun ay yil degerleri yaziliyor */
	        tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

		  //en buyuk fatura degeri okunup bir atrýlýp yazacak
		  strcpy(query, "select max(fatura_no) from faturalar");
		  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		  sprintf(max_fatura_no_str,"%s",pgsql_kayit_field());
		  max_fatura_no=atoi(max_fatura_no_str);
		  sprintf(max_fatura_no_str,"%i",max_fatura_no+1);
		  set_field_buffer(field[4],0,max_fatura_no_str);			

	      durum = 1;	//su anda yeni kayýt konumuna gecildi
	    }

	  break;

	case KEY_F (4):	/*sil */
	  //silme hakký kontrolu
	  if (haklari_kontrol_et (3) == 0)
	    {
	      mesaj ( _("Silme hakkýnýz yok.!!!" ) );
		  form_driver (my_form, REQ_NEXT_FIELD);
	      form_driver (my_form, REQ_PREV_FIELD);
	      break;
	    }
	  donen_deger = onayla (kayit_silme_onayla);

	  if (donen_deger == 1)
	    {			
		//cari hesabýna iþlenecktir
		
		sprintf (query, "select firma_adi, toplam_tutar from faturalar where fatura_no='%s'",
		g_strstrip(field_buffer (field[4], 0)) );
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
		sprintf(firma_adi_str, "%s", pgsql_kayit_field() );
		toplam_tutar=atoll(pgsql_kayit_field());
		
		//adamýn faturasý silindi borcuna islenecek
		cari_miktar_isle(firma_adi_str, toplam_tutar, 1);
			
	    sprintf (query, "delete from faturagelen where belge_no='%s' ",
		g_strstrip(field_buffer (field[4], 0))  );
					
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}			

		sprintf (query, "delete from faturalar where fatura_no='%s' ",
		   g_strstrip(field_buffer (field[4], 0))  );
		
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}			
		
		
		
		/********************************/
		//silme iþleminden sonra en son kayýt uzerin ekonumlan

  row_no_buyuk_tur ("faturalar", "0");
  sprintf (query,
  "select row_no, tarih, fatura_no,irsaliye_no, firma_adi, toplam, kdv, toplam_tutar from faturalar where row_no=%i and tur='0' ",
  row_no - 1);
  
  pgsql_sorgula(query);
  
//row no
  set_field_buffer (field[0], 0, pgsql_kayit_field() );
//tarih
  sql_tarih_ayir ( pgsql_kayit_field() );
  set_field_buffer (field[1], 0, sql_gun);
  set_field_buffer (field[2], 0, sql_ay);
  set_field_buffer (field[3], 0, sql_yil);
//fatura ve irsaliye no
  set_field_buffer (field[4], 0, pgsql_kayit_field() );
  set_field_buffer (field[5], 0, pgsql_kayit_field() );
//firma adi
  set_field_buffer (field[6], 0, pgsql_kayit_field() );
//toplam
  set_field_buffer (field[7], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//kdv
  set_field_buffer (field[8], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//toplam_tutar
  set_field_buffer (field[9], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );

  sprintf (query,
  "select malzeme_kodu, aciklama, miktar, birim_tip, kdv_oran, birim_fiyat,tutar from faturagelen where tarih='%s-%s-%s'  and belge_no='%s' and irsaliye_no='%s' ",
  g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
  g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[4], 0)),
  g_strstrip(field_buffer (field[5], 0))  );


//burada kontrol yapma cunku ilk kayit olabilir
  pgsql_sorgula(query);
  
  i = 13;
  for (j=0;j<pgsql_row_miktar;j++)
    {
      set_field_buffer (field[i], 0, pgsql_kayit_field() );
      set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
      set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
	  set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
	  set_field_buffer (field[i + 4], 0, pgsql_kayit_field() );
	  set_field_buffer (field[i + 5], 0, yazidan_paraya_cevir (pgsql_kayit_field()) );
      set_field_buffer (field[i + 6], 0, yazidan_paraya_cevir (pgsql_kayit_field()) );
      i = i + 7;
    }


	      refresh ();
	    }
	  break;

	case KEY_NPAGE:	/*geri kayýt */
		if (durum != 0)
	    	break;

	  rownno = atoi (field_buffer (field[0], 0));
	  //en kucuk mu diye bakmak lazým eger en kucuk ise break
	  row_no_kucuk_tur ("faturalar", "0");
	  if (rownno == row_no)
	    break;

//rownno dan kucuk ama en buyuk kayýt bulunacak
	  sprintf (query,
	  "select row_no, tarih, fatura_no,irsaliye_no, firma_adi, toplam, kdv, toplam_tutar,isk_oran, isk_tutar from faturalar where row_no < %i and tur='0' order by row_no desc",
	  rownno);
	
	  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	  
//row no
	  set_field_buffer (field[0], 0, pgsql_kayit_field() );
//tarih
	  sql_tarih_ayir ( pgsql_kayit_field() );
	  set_field_buffer (field[1], 0, sql_gun);
	  set_field_buffer (field[2], 0, sql_ay);
	  set_field_buffer (field[3], 0, sql_yil);
//fatura ve irsaliye no
	  set_field_buffer (field[4], 0, pgsql_kayit_field() );
	  set_field_buffer (field[5], 0, pgsql_kayit_field() );
//firma adi
	  set_field_buffer (field[6], 0, pgsql_kayit_field() );
//toplam
		set_field_buffer (field[7], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//kdv
  set_field_buffer (field[8], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//toplam_tutar
  set_field_buffer (field[9], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//isk_oran
  set_field_buffer (field[11], 0, pgsql_kayit_field() );
//isk_tutar
  set_field_buffer (field[12], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
//ara toplam
  sprintf(toplam_str,"%lli", atoll(paradan_yaziya_cevir(field_buffer(field[7],0)))+
	atoll( paradan_yaziya_cevir(field_buffer(field[12],0) )  ) );
  set_field_buffer (field[10], 0, yazidan_paraya_cevir(toplam_str) );

	  sprintf (query,
	   "select malzeme_kodu, aciklama, miktar, birim_tip, kdv_oran, birim_fiyat, tutar from faturagelen where tarih='%s-%s-%s'  and belge_no='%s' and irsaliye_no='%s' ",
	   g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
	   g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[4], 0)),
	   g_strstrip(field_buffer (field[5], 0))  );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

	  i = 13;
	  for (j=0;j<pgsql_row_miktar;j++)
	    {
	      set_field_buffer (field[i], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 4], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 5], 0, yazidan_paraya_cevir ( pgsql_kayit_field() ) );
	      set_field_buffer (field[i + 6], 0, yazidan_paraya_cevir ( pgsql_kayit_field() ) );
	      i = i + 7;
	    }

	  //geri kalan alanlar bosaltýlýr
	  while (i < 118)
	    {
	    set_field_buffer (field[i], 0, "");
		i++;
	    }

	  break;


/***********************************************************************/

	case KEY_PPAGE:	/*ileri kayýt */

	  if (durum != 0)
	    break;

	  //en buyukmu diye bakmak lþazim eger en buyu ise break
	  row_no_buyuk_tur ("faturalar", "0");
	  rownno = atoi (field_buffer (field[0], 0));
	  
	  if (rownno == (row_no - 1))    break;

	  //rownno dan kucuk ama en buyuk kayýt bulunacak
	  sprintf (query,
	  "select row_no, tarih, fatura_no,irsaliye_no, firma_adi, toplam, kdv, toplam_tutar, isk_oran, isk_tutar from faturalar where row_no > %i and tur='0' order by row_no asc",
	  rownno);

	  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	  
	  //row no
	  set_field_buffer (field[0], 0, pgsql_kayit_field() );

	  //tarih
	  sql_tarih_ayir ( pgsql_kayit_field() );
	  set_field_buffer (field[1], 0, sql_gun);
	  set_field_buffer (field[2], 0, sql_ay);
	  set_field_buffer (field[3], 0, sql_yil);
	  //fatura ve irsaliye no
	  set_field_buffer (field[4], 0, pgsql_kayit_field() );
	  set_field_buffer (field[5], 0, pgsql_kayit_field() );
	  //firma adi
	  set_field_buffer (field[6], 0, pgsql_kayit_field() );
	  //toplam
	  set_field_buffer (field[7], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
		//kdv
		  set_field_buffer (field[8], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
		//toplam_tutar
		  set_field_buffer (field[9], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
		//isk_oran
		  set_field_buffer (field[11], 0, pgsql_kayit_field() );
		//isk_tutar
		  set_field_buffer (field[12], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );
		//ara toplam
		  sprintf(toplam_str,"%lli", atoll(paradan_yaziya_cevir(field_buffer(field[7],0)))+
			atoll( paradan_yaziya_cevir(field_buffer(field[12],0) )  ) );
		  set_field_buffer (field[10], 0, yazidan_paraya_cevir(toplam_str) );
 
	  sprintf (query,
	   "select malzeme_kodu, aciklama, miktar,birim_tip, kdv_oran, birim_fiyat, tutar from faturagelen where tarih='%s-%s-%s'  and belge_no='%s' and irsaliye_no='%s' ",
	   g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
	   g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[4], 0)),
	   g_strstrip(field_buffer (field[5], 0))  );

	  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	  
	  i = 13;
	  for (j=0;j<pgsql_row_miktar;j++)
	    {
	      set_field_buffer (field[i], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
	      set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
		  set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
		  set_field_buffer (field[i + 4], 0, pgsql_kayit_field() );
		  set_field_buffer (field[i + 5], 0, yazidan_paraya_cevir ( pgsql_kayit_field() ) );
	      set_field_buffer (field[i + 6], 0, yazidan_paraya_cevir ( pgsql_kayit_field() ) );
	      i = i + 7;
	    }

	  //geri kalan alanlar bosaltýlýr
	  while (i < 118)
	    {
	      set_field_buffer (field[i], 0, "");	      
	      i = i + 1;
	    }


	  break;

	
	default:
		form_driver(my_form, ch);
		break;

    }//switch sonu
	    
}//while sonu

for (i = 0; i < 118; i++) free_field (field[i]);

}//fonksiyon sonu

/*********************************************************/
