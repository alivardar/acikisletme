
int cari ()
{
  ITEM **my_items;
  int ch, c;
  MENU *cari_menu;
  WINDOW *cari_win;
  int n_choices, i;
  ITEM *cur_item;

  char *choices[] = {
    _("Collect and Payment Receipt"),
    _("Receipt "),
    _("Gelen Fatura ��lemleri"),
    _("Bill Kesim ��lemleri"),
    _("Yeni Cari Hesap Kartonu"),
    _("Cari Hesap Kartonlar�"),
    _("Back"),
    (char *) NULL,
  };
 
  init_pair (1, COLOR_WHITE, COLOR_RED);
  init_pair (2, COLOR_WHITE, COLOR_BLUE);

  ekran_temizle();

  baslik_goruntule ();

  n_choices = ARRAY_SIZE (choices);
  my_items = (ITEM **) calloc (n_choices, sizeof (ITEM *));
  for (i = 0; i < n_choices; ++i)
    my_items[i] = new_item (choices[i], " ");

  cari_menu = new_menu ((ITEM **) my_items);

  cari_win = newwin (15, 50, 5, 10);
  keypad (cari_win, TRUE);

  set_menu_win (cari_menu, cari_win);
  set_menu_sub (cari_menu, derwin (cari_win, 10, 40, 4, 2));

  set_menu_mark (cari_menu, mark);

  box (cari_win, 0, 0);
  print_in_middle (cari_win, 1, 0, 45, _("Currency"), COLOR_PAIR (1));
  mvwaddch (cari_win, 2, 0, ACS_LTEE);
  mvwhline (cari_win, 2, 1, ACS_HLINE, 48);
  mvwaddch (cari_win, 2, 49, ACS_RTEE);

  post_menu (cari_menu);
  wrefresh (cari_win);

  while ((ch != 27))
    {
      ch = wgetch (cari_win);
      switch (ch)
	{
	case KEY_F (10):
	  ch = 27;
	  break;
	case KEY_DOWN:
	  menu_driver (cari_menu, REQ_DOWN_ITEM);
	  break;
	case KEY_UP:
	  menu_driver (cari_menu, REQ_UP_ITEM);
	  break;
	case 10:
	  cur_item = current_item (cari_menu);
	  switch (item_index (cur_item) + 1)
	    {
	    case 1:/*Tahsilat - Tediye Makbuzu */

	      tahsil_tediye_makbuz_yeni ();

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;
	    case 2:/*Eski makbuz i�lemleri */

	      tahsil_tediye_makbuz_eski (0);

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;

	    case 3:/*Gelen faturalar */

	      fatura_gelen_islemleri ();

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;

	    case 4:		/*Fatura kesim i�lemleri */

	      fatura_kesim_islemleri ();

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;

	    case 5:		/*yeni cari kartonu */
	      cari_karton_yeni ();

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;

	    case 6:		/*eski cari kartonu */
	      cari_karton_eski (0);

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;

	    case 7:		/*ana menu */

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);

	      for (i = 0; i < n_choices; ++i)
		  free_item (my_items[i]);

	      return 0;

	    }			//switch sonu
	  break;

	  refresh ();
	  pos_menu_cursor (cari_menu);
	}
      wrefresh (cari_win);
    }

/*esc bas�l�nca kapat�l�r*/
 
  for (i = 0; i < n_choices; ++i) free_item (my_items[i]);
  delwin (cari_win);
  return 0;
}


/******************************************************************************/

int cari_karton_yeni ()
{
  char query[2000] = "";
  char tarih[10] = "";

  FIELD *field[9];
  FORM *my_form;
  int i;
  int ch=0;

  touchwin (ana_win);
  wrefresh (ana_win);
  
  ekran_temizle();

/*Kay�t i�leminden sonra row no de�eri verilir ekrana yaz�l�r*/
/*firmalar tablosu*/
/*row_no*/
  field[0] = new_field (1, 8, 4, 5, 0, 0);
/*bilanco_no*/
  field[1] = new_field (1, 45, 4, 30, 0, 0);
/*firma_adi*/
  field[2] = new_field (1, 60, 6, 15, 0, 0);
/*adres*/
  field[3] = new_field (1, 65, 8, 10, 0, 0);
/*vergi_dairesi*/
  field[4] = new_field (1, 55, 10, 20, 0, 0);
/*vergi_no*/
  field[5] = new_field (1, 60, 12, 15, 0, 0);
/*telefon*/
  field[6] = new_field (1, 55, 14, 20, 0, 0);
/*fax*/
  field[7] = new_field (1, 55, 16, 20, 0, 0);
/*aciklama*/
  field[8] = new_field (1, 30, 18, 15, 0, 0);
/*para birimi*/
  field[9] = new_field (1, 5, 18, 70, 0, 0);


  field[10] = NULL;

  for (i = 0; i < 10; i++)
    {
//set_field_fore(field[i], COLOR_PAIR(1));
//set_field_back(field[i], COLOR_PAIR(2));
//set_field_back(field[i], A_UNDERLINE);
      field_opts_off (field[i], O_AUTOSKIP);
/*hiazlama fonksiyonu*/
//set_field_just(field[i], JUSTIFY_RIGHT);
    }

  field_opts_off (field[0], O_ACTIVE);
  form_driver (field[1], REQ_FIRST_PAGE);

  my_form = new_form (field);
  post_form (my_form);

for (i=0;i<10;i++)
{
set_field_fore(field[i], COLOR_PAIR(2));
set_field_back(field[i], COLOR_PAIR(2));
}

  attron (COLOR_PAIR (2));
  mvprintw (1, 0,
  _("                         Yeni Cari Hesap Kartonu                                ") );
  baslik_goruntule ();
  attroff (COLOR_PAIR (2));

/*toplam 3 sat�r*/
  mvprintw (4, 1, _("No:") );
  mvprintw (4, 15, _("M��teri no:") );
  mvprintw (6, 1, _("Firma Ad�:") );
  mvprintw (8, 1, _("Adres:") );
  mvprintw (10, 1, _("Vergi Dairesi:") );
  mvprintw (12, 1, _("Vergi No:") );
  mvprintw (14, 1, _("Telefon:") );
  mvprintw (16, 1, _("Fax:") );
  mvprintw (18, 1, _("A��klama:") );
  mvprintw (18, 55, _("Para Birimi:") );

mvprintw(LINES - 3, 0, "                  F2->Save      F3->New      ESC->Exit ");

/*en buyuk olan row_no bulunur */
  row_no_buyuk ("firmalar");
  set_field_buffer (field[0], 0, rowno);

/*Para birimi yaz�lacak*/
if ( strstr(getenv("LANG"), "tr_TR") )   set_field_buffer (field[9], 0, "YTL"); else
if ( strstr(getenv("LANG"), "en_US") )  set_field_buffer (field[9], 0, "$"); else
set_field_buffer (field[9], 0, "EUR");
	
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

	case 8:		/*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r */
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;

	case KEY_BACKSPACE:
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;

	case 9:		/*TAB tusuna bas�lmas� durumunda sonraki field konumlan */
	  form_driver (my_form, REQ_PREV_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case 10:		/*ENTER tu�una bas�lmas� durumunda sonraki feild gececek */
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
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_UP:
	  form_driver (my_form, REQ_PREV_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_LEFT:
	  form_driver (my_form, REQ_LEFT_CHAR);
	  break;
	case KEY_RIGHT:
	  form_driver (my_form, REQ_RIGHT_CHAR);
	  break;

	case KEY_F (2):	

/*Kay�t yapmadan evvel snaroki alana gecilmeli okuma yapm�yor aksi halde*/
	  form_driver (my_form, REQ_NEXT_FIELD);

sprintf (query,
"insert into firmalar (bilanco_no, firma_adi, adres, vergi_dairesi, vergi_no, telefon, fax, aciklama, para_birim, bakiye) values ('%s', '%s' ,'%s' ,'%s' ,'%s' ,'%s' ,'%s', '%s', '%s', 0 )",
/*row_no, bilanco_no*/
g_strstrip( field_buffer (field[1], 0) ),
/*firma_adi, adres*/
g_strstrip( field_buffer (field[2], 0) ), g_strstrip( field_buffer (field[3], 0) ),
/*vergi_dairesi, vergi_no*/
g_strstrip( field_buffer (field[4], 0) ), g_strstrip( field_buffer (field[5], 0) ),
/*telefon, fax, a��klama*/
g_strstrip( field_buffer (field[6], 0) ), g_strstrip( field_buffer (field[7], 0) ),
g_strstrip( field_buffer (field[8], 0) ), g_strstrip( field_buffer (field[9], 0) )  );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	
	  donen_deger = onayla (yeni_kayit_istiyormusunuz);
	  if (donen_deger == 0)	    return;
		  
	  if (donen_deger == 1)
	    {
	      /*evet de�eri girildi */
	      /*row_no alan� ogrenilerek 1 art�l�p ekrana yaz�lacakt�r */
	      row_no_buyuk ("firmalar");
	      set_field_buffer (field[0], 0, rowno);
	      /*diger alanlar ise bosalt�lacakt�r */
	      set_field_buffer (field[1], 0, "");
	      set_field_buffer (field[2], 0, "");
	      set_field_buffer (field[3], 0, "");
	      set_field_buffer (field[4], 0, "");
	      set_field_buffer (field[5], 0, "");
	      set_field_buffer (field[6], 0, "");
	      set_field_buffer (field[7], 0, "");
	      set_field_buffer (field[8], 0, "");		  
	    }

	  break;

	case KEY_F (3):	/*f3 yeni i�lemleri i�in */

	  donen_deger = onayla (yeni_kayit);
	  if (donen_deger == 0)
	    return;
	  if (donen_deger == 1)
	    {
	      /*evet de�eri girildi */
	      /*row_no alan� ogrenilerek 1 art�l�p ekrana yaz�lacakt�r */
	      row_no_buyuk ("firmalar");
	      set_field_buffer (field[0], 0, rowno);
	      /*diger alanlar ise bosalt�lacakt�r */
	      set_field_buffer (field[1], 0, "");
	      set_field_buffer (field[2], 0, "");
	      set_field_buffer (field[3], 0, "");
	      set_field_buffer (field[4], 0, "");
	      set_field_buffer (field[5], 0, "");
	      set_field_buffer (field[6], 0, "");
	      set_field_buffer (field[7], 0, "");
	      set_field_buffer (field[8], 0, "");
	    }

	  break;

	default:
	  form_driver (my_form, ch);
	  break;
	}

    }

for (i = 0; i < 10; i++);  free_field (field[i]);
return;
}

/********************************************************************/

int cari_karton_eski (unsigned long int kayit_no)
{
  char query[2000] = "";

  char gecici_yazi[1000] = "";
 
  int ch, i, c;

  ITEM *my_items[20];
  ITEM *cur_item;
  MENU *my_menu;
  WINDOW *my_menu_win;

  char *deger[20][60];
  char *deger2[20][60];
  int *donen_deger;
  
  touchwin (ana_win);
  wrefresh (ana_win);

  ekran_temizle();
  
  row_no_buyuk ("firmalar");
  if (row_no < kayit_no)  kayit_no = kayit_no - 15;

/*burada mysql den istenen tablo ve field alan� okunacak ve de�i�kenler d�zenlenecek*/
  sprintf (query, "select row_no, firma_adi from firmalar order by firma_adi limit 15 offset %d", kayit_no);

if (strlen (g_strstrip(giris)) > 2)
sprintf (query,
"select row_no, firma_adi from firmalar where firma_adi like '%%%s%%' order by firma_adi limit 15 offset 0",
giris);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

for (i=0;i<pgsql_row_miktar;i++)
    {
      sprintf (deger[i], "%s", pgsql_kayit_field() );
      sprintf (deger2[i], "%s", pgsql_kayit_field() );
      my_items[i] = new_item (&deger[i], &deger2[i]);      
    }

  my_items[i] = (ITEM *) NULL;
  my_menu = new_menu ((ITEM **) my_items);

  /*3 sat�r 3 sutun 15 sat�r 60 sutun pencere */
  my_menu_win = newwin (21, 75, 1, 3);

  keypad (my_menu_win, TRUE);

  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 15, 70, 4, 1));
  set_menu_format (my_menu, 15, 1);

  set_menu_mark (my_menu, mark);

  box (my_menu_win, 0, 0);
  //print_in_middle(my_menu_win, 1, 0, 75, _("  F4-> D�zelt   F5-> Ara   F8-> Sil   ESC-> ��k��   Se�iminizi Yap�n�z "), COLOR_PAIR(1));
  print_in_middle (my_menu_win, 1, 0, 75,
		   _("  F4-> D�zelt  F5-> Ara  F8-> Sil  ESC-> ��k��  Se�iminizi Yap�n�z "),
		   COLOR_PAIR (1));

  print_in_middle (my_menu_win, 20, 0, 75,
		   _("  PAGE UP -> �nceki Sayfa     PAGE DOWN -> Sonraki Sayfa "),
		   COLOR_PAIR (1));

  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, 80);
  mvwaddch (my_menu_win, 2, 79, ACS_RTEE);

  post_menu (my_menu);
  wrefresh (my_menu_win);

/*buras� �al��maz ise hata veriyor ve de�eri okuyam�yor*/
  menu_driver (my_menu, REQ_DOWN_ITEM);
  cur_item = current_item (my_menu);
  wrefresh (my_menu_win);

  while ((ch != 27))
    {
      ch = getch ();
      switch (ch)
	{
	case KEY_F (10):
	  ch = 27;
	  break;

	case KEY_DOWN:
	  menu_driver (my_menu, REQ_DOWN_ITEM);
	  break;
	case KEY_UP:
	  menu_driver (my_menu, REQ_UP_ITEM);
	  break;
	case KEY_F (4):	/* F4 Stok kartonu duzenleme ekran� */
	  sprintf (gecici_yazi, "%s", item_name (cur_item));
	  cari_karton_duzenle (gecici_yazi);
	  return 0;
	  break;

	case KEY_F (5):	/*arama tusu bas�l�rsa */

	  aranacak_kelime
	    ( _("�irket ad� veya yak�n bir de�er giriniz. D�zg�n   s�ralama i�in bo� b�rak�n�z. ") );
	  cari_karton_eski (0);
	  return 0;

	case KEY_F (8):	/*sil tusu bas�l�rsa f8 */

	  //silme hakk� kontrolu
	  if (haklari_kontrol_et (3) == 0)
	    {
	      mesaj ( _("Silme hakk�n�z yok.!!!") );
	      break;
	    }

	  donen_deger = onayla (kayit_silme_onayla);
	  if (donen_deger == 1)
	    {
	      /*Silme i�lemi onayland� ilgili olan row_no bulunacak ve kay�t silinecek daha
	         sonra menu refresh edilmelidir */
	      sprintf (query, "delete from firmalar where row_no='%s'", item_name (cur_item));
			
	      if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	      
	      for (c = 0; c < i; ++c)	free_item (my_items[c]);

	      cari_karton_eski (kayit_no);
	      refresh;

	      //return 0;
	    }
	  break;
/*page up page down yaz�lacak*/

	case KEY_NPAGE:	/* Page down bas�l�rsa */

	  kayit_no = kayit_no + 15;

	  cari_karton_eski (kayit_no);
	  return 0;
	  break;

	case KEY_PPAGE:	/* Page up bas�l�rsa */
	  if (kayit_no > 14)
	    {
	      kayit_no = kayit_no - 15;
	    }

	  cari_karton_eski (kayit_no);

	  return 0;

	  break;
	}
      cur_item = current_item (my_menu);
      wrefresh (my_menu_win);
    }

  /* uzerinde bulunan menu degerinini isim degeri yani ilk deger rowno */
  //sprintf(donen,"%s",item_name(cur_item));



}

/******************************************************************************/

int cari_karton_duzenle (unsigned long int kayit_no)
{
/*bu fonksiyon stok_karton_eski taraf�ndan menude duzenle t�klan�nca �a�r�l�r
gonderilen rowno ile duzenleme ekran� a��l�r*/
FIELD *field[11];
FORM *my_form;
int ch;
char firma_adi_str[100] = "";
char para_yazi_str[50];
	
touchwin (ana_win);
wrefresh (ana_win);

ekran_temizle();

//set_field_type(field[2], TYPE_ALNUM);
//set_field_type(field[6], TYPE_INTEGER);

/*Kay�t i�elmeinden sonra row no de�eri verilir ekrana yaz�l�r*/
	
/*stokkart tablosu*/

/*row_no*/
  field[0] = new_field (1, 8, 4, 5, 0, 0);
/*bilanco_no*/
  field[1] = new_field (1, 45, 4, 30, 0, 0);
/*firma_adi*/
  field[2] = new_field (1, 60, 6, 15, 0, 0);
/*adres*/
  field[3] = new_field (1, 65, 8, 10, 0, 0);
/*vergi_dairesi*/
  field[4] = new_field (1, 55, 10, 20, 0, 0);
/*vergi_no*/
  field[5] = new_field (1, 55, 12, 20, 0, 0);
/*telefon*/
  field[6] = new_field (1, 55, 14, 20, 0, 0);
/*fax*/
  field[7] = new_field (1, 55, 16, 20, 0, 0);
/*ac�klama*/
  field[8] = new_field (1, 55, 18, 20, 0, 0);
/*bakiye*/
  field[9] = new_field (1, 25, 20, 20, 0, 0);
/*durum*/
  field[10] = new_field (1, 3, 20, 72, 0, 0);

  field[11] = NULL;

  set_field_just (field[10], JUSTIFY_CENTER);
  set_field_just (field[9], JUSTIFY_RIGHT);

  field_opts_off (field[0], O_ACTIVE);
  field_opts_off (field[9], O_ACTIVE);
  field_opts_off (field[10], O_ACTIVE);

  form_driver (field[1], REQ_FIRST_PAGE);

  my_form = new_form (field);
  post_form (my_form);

  for (i=0;i<11;i++)
	{
	field_opts_off (field[i], O_AUTOSKIP);
	set_field_fore(field[i], COLOR_PAIR(2));
	set_field_back(field[i], COLOR_PAIR(2));
	}

  attron (COLOR_PAIR (2));
  mvprintw (1, 0,
	    _("                    Cari Hesap D�zenleme Ekran�                                ") );
  baslik_goruntule ();
  attroff (COLOR_PAIR (2));

/*toplam 3 sat�r*/
  mvprintw (4, 1, _("No:") );
  mvprintw (4, 15, _("Bilan�o no:") ); 
  mvprintw (6, 1, _("Firma Ad�:") );
  mvprintw (8, 1, _("Adres:") );
  mvprintw (10, 1, _("Vergi Dairesi:") );
  mvprintw (12, 1, _("Vergi No:") );
  mvprintw (14, 1, _("Telefon:") );
  mvprintw (16, 1, _("Fax:") );
  mvprintw (18, 1, _("A��klama:") );
  mvprintw (20, 1, _("Bakiye:") );
  mvprintw (20, 60, _("B/A:") );

mvprintw(LINES - 3, 0, _("                     F2->Kaydet                 ESC->��k��") );


/*kayit_no bilgiisi row_no bilgisidiri bu bilgi ile sql den bilgi �ekilecek ve 
field lar i�ine yerle�tirilecektir*/

sprintf (query,
"select row_no, bilanco_no, firma_adi, adres, vergi_dairesi,vergi_no, telefon, fax, aciklama, bakiye, para_birim, durum from firmalar where row_no=%s ",
kayit_no);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

for (i=0;i<pgsql_row_miktar;i++)
    {
      set_field_buffer (field[0], 0, pgsql_kayit_field() );
      set_field_buffer (field[1], 0, pgsql_kayit_field() );
      set_field_buffer (field[2], 0, pgsql_kayit_field() );
      set_field_buffer (field[3], 0, pgsql_kayit_field() );
      set_field_buffer (field[4], 0, pgsql_kayit_field() );
      set_field_buffer (field[5], 0, pgsql_kayit_field() );
      set_field_buffer (field[6], 0, pgsql_kayit_field() );
      set_field_buffer (field[7], 0, pgsql_kayit_field() );
      set_field_buffer (field[8], 0, pgsql_kayit_field() );
	  sprintf(para_yazi_str, "%s", yazidan_paraya_cevir ( pgsql_kayit_field() ) );
	  sprintf(para_yazi_str, "%s%s", para_yazi_str, g_strstrip(pgsql_kayit_field()) );
      set_field_buffer (field[9], 0, para_yazi_str );
      set_field_buffer (field[10], 0, pgsql_kayit_field() );
    }

//eski firma ad� saklan�r
sprintf(firma_adi_str, "%s", g_strstrip(field_buffer (field[2], 0) ) );

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
	case 8:	/*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r */
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case KEY_BACKSPACE:
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case 9:	/*TAB tusuna bas�lmas� durumunda sonraki field konumlan */
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case 10:/*ENTER tu�una bas�lmas� durumunda sonraki feild gececek */
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_DC: /*del tusu */
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case KEY_HOME:
	  form_driver (my_form, REQ_BEG_LINE);
	  break;
	case KEY_END:
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_DOWN:
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_UP:
	  form_driver (my_form, REQ_PREV_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_LEFT:
	  form_driver (my_form, REQ_LEFT_CHAR);
	  break;
	case KEY_RIGHT:
	  form_driver (my_form, REQ_RIGHT_CHAR);
	  break;
	case KEY_F (2):
	  /*Kay�t yapmadan evvel snaroki alana gecilmeli okuma yapm�yor aksi halde */
	  form_driver (my_form, REQ_NEXT_FIELD);

/* update i�lemi yap�l�yor */
sprintf (query,
"update firmalar set bilanco_no='%s', firma_adi='%s', adres='%s', vergi_dairesi='%s',vergi_no='%s', telefon='%s', fax='%s', aciklama='%s'  where (row_no=%s) ",
/*bakiye ve durum guncelleme yap�lamaz*/
/*bilanco_no*/
g_strstrip(  field_buffer (field[1], 0) ),
/*firma_adi, adres*/
g_strstrip( field_buffer (field[2], 0) ), g_strstrip( field_buffer (field[3], 0) ),
/* vergi_dairesi, vergi_no*/
g_strstrip( field_buffer (field[4], 0) ), g_strstrip( field_buffer (field[5], 0) ),
/* telefon,fax, aciklama */
g_strstrip( field_buffer (field[6], 0) ), g_strstrip( field_buffer (field[7], 0) ),
g_strstrip( field_buffer (field[8], 0) ),
kayit_no);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//diger tablolarda firma adi update edilecek
//eski firma_adi

//faturagelen tablosu
sprintf (query, "update faturagelen set kimden='%s' where kimden='%s'",  g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//faturakesilen tablosu
sprintf (query, "update faturakesilen set kimden='%s' where kimden='%s'", g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

// faturalar tablosu
sprintf (query, "update faturalar set firma_adi='%s' where firma_adi='%s'",  g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//irsaliyeler tablosu
sprintf (query, "update irsaliyeler set firma_adi='%s' where firma_adi='%s'", g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//makbuz tablosu
sprintf (query, "update makbuz set firma_adi='%s' where firma_adi='%s'",  g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

// stokgelen tablosu
sprintf (query, "update stokgelen set kimden='%s' where kimden='%s'",  g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

//stokgiden tablosu
sprintf (query, "update stokgiden set kime='%s' where kime='%s'",  g_strstrip( field_buffer (field[2], 0) ), firma_adi_str  );
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

mesaj (duzenleme_basarili);

for (i = 0; i < 10; i++);  free_field (field[i]);
	  cari_karton_eski (0);
	  break;
/*f3 duzeltme i�lemlerinin sonu*/

	default:
	  form_driver (my_form, ch);
	  break;
	}

    }
  for (i = 0; i < 11; i++) free_field (field[i]);
}

/******************************************************************************/

int tahsil_tediye_makbuz_yeni ()
{
WINDOW *makbuz_win;
char query[2000] = "";
char tarih[10] = "";

FIELD *field[8];	
FORM *my_form_makbuz;
int i, j, k, satir, sira;	
int ch=0;

int tahsil = 1;
char tur[6] = "tahsil";

long int rowno_miktar;
char deger[50];
unsigned int max_makbuz_no;
char max_makbuz_no_str[20];

char para_str[50];
//makbuz yard�rma de�i�kenleri
char baslik_str[100];
char sirket_adi_str[100];
char adres_str[100];
char vergi_dairesi_str[100];
char vergi_no_str[100];
char telefon1_str[100];
char fax_str[100];
char tarih_str[100];
char makbuz_str[100];
char tutar_str[100];
char yazi1_str[100];
char yazi2_str[100];
char yazi3_str[100];
char aciklama_str[100];
char bilgi_str[200];

  
 ekran_temizle();

  makbuz_win = newwin (24, 80, 0, 0);
  keypad (makbuz_win, TRUE);

/*row_no int*/
/*Kay�t i�leminden sonra row no de�eri verilir ekrana yaz�l�r*/
  field[0] = new_field (1, 8, 3, 5, 0, 0);
/*tarih date*/
  field[1] = new_field (1, 2, 3, 29, 0, 0);
  field[2] = new_field (1, 2, 3, 32, 0, 0);
  field[3] = new_field (1, 4, 3, 35, 0, 0);
/*belge_no char*/
  field[4] = new_field (1, 15, 3, 60, 0, 0);

/*firma*/
  field[5] = new_field (1, 60, 5, 15, 0, 0);

/*ac�klama*/
  field[6] = new_field (1, 60, 7, 15, 0, 0);

/*tutar*/
  field[7] = new_field (1, 25, 9, 15, 0, 0);

  field[8] = NULL;

//tarih alan�na int girmek zorundalar
  set_field_type (field[1], TYPE_INTEGER);
  set_field_type (field[2], TYPE_INTEGER);
  set_field_type (field[3], TYPE_INTEGER);
  
  set_field_type (field[7], TYPE_NUMERIC);

  set_field_just (field[0], JUSTIFY_RIGHT);
  set_field_just (field[5], JUSTIFY_LEFT);
  set_field_just (field[6], JUSTIFY_LEFT);
  set_field_just (field[7], JUSTIFY_RIGHT);

/*yaz� yaz�lamaz sadece gorme modunda ilk ac�l�nca*/
  field_opts_off (field[0], O_ACTIVE);
/*firmay aelle yaz� yaz�lmaz*/
  field_opts_off (field[5], O_EDIT);

  my_form_makbuz = new_form (field);

//  set_form_win (my_form_makbuz, makbuz_win);
//  set_form_sub (my_form_makbuz, derwin (makbuz_win, 25, 80, 0, 0));
  post_form (my_form_makbuz);



  attron (COLOR_PAIR (1));
  mvprintw (1, 0,
	    _("                            Yeni Tahsilat Makbuzu                               ") );
  attroff (COLOR_PAIR (1));

 //baslik_goruntule ();
  
for (i=4;i<8;i++)
{
set_field_fore(field[i], COLOR_PAIR(2));
set_field_back(field[i], COLOR_PAIR(2));
}

/*toplam 3 sat�r*/
  mvprintw (3, 1, _("No :") );
  mvprintw (3, 20, _("Tarih :") );
  mvprintw (3, 31, "/");
  mvprintw (3, 34, "/");
  mvprintw (3, 45, _("Makbuz No :") );

  mvprintw (5, 1, _("Firma   :") );

  mvprintw (7, 1, _("A��klama:") );
  mvprintw (9, 1, _("Tutar   :") );

  mvprintw(LINES - 5, 0, _("    F5->Makbuz yazd�r") );
  mvprintw(LINES - 3, 0, _("    F2->Kaydet   F3->Yeni  F8->Firma Se�imi   F7->Tahsil/Tediye   ESC->��k��") ); 

/*tarih i�ine degerler yaz�l�yor*/
/*gun ay yil degerleri yaziliyor*/
  tarih_ayarla ();

  set_field_buffer (field[1], 0, gun);
  set_field_buffer (field[2], 0, ay);
  set_field_buffer (field[3], 0, yil);

 //en buyuk makbuz degeri okunup bir atr�l�p yazacak
  strcpy(query, "select max(belge_no) from makbuz");
  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
  sprintf(max_makbuz_no_str,"%s",pgsql_kayit_field());
  max_makbuz_no=atoi(max_makbuz_no_str);
  sprintf(max_makbuz_no_str,"%i",max_makbuz_no+1);
  set_field_buffer(field[4],0,max_makbuz_no_str);			

//ilk ac�l�nca en son kay�t uzerine konumlan
  row_no_buyuk ("makbuz");
  set_field_buffer (field[0], 0, rowno);

//paraya ilk deger 
//set_field_buffer(field[7], 0, "0");

  touchwin(makbuz_win);
  wrefresh(makbuz_win);
  
  //refresh();
  
  ch=0;
  
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
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);
	  refresh();
	  break;
	case 8:		/*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r konsolda 8 xterm key_backspace */
	  form_driver (my_form_makbuz, REQ_PREV_CHAR);
	  form_driver (my_form_makbuz, REQ_DEL_CHAR);
	  break;
	case KEY_BACKSPACE:
	  form_driver (my_form_makbuz, REQ_PREV_CHAR);
	  form_driver (my_form_makbuz, REQ_DEL_CHAR);
	  break;	
	case 9:		/*TAB tusuna bas�lmas� durumunda sonraki field konumlan */
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);
	  break;
	case 10:		/*ENTER tu�una bas�lmas� durumunda sonraki feild gececek */
 
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);		
	  
	  //para format�na cevrilecek	  	  
	  sprintf(para_str,"%s", paradan_yaziya_cevir(g_strstrip(field_buffer(field[7], 0)))  );
	  set_field_buffer(field[7], 0,  yazidan_paraya_cevir(para_str) );

	  break;
	case KEY_DC:		/*del tusu */
	  form_driver (my_form_makbuz, REQ_DEL_CHAR);
	  break;
	case KEY_HOME:
	  form_driver (my_form_makbuz, REQ_BEG_LINE);
	  break;
	case KEY_END:
	  form_driver (my_form_makbuz, REQ_END_LINE);
	  break;
	case KEY_DOWN:
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);
	  break;
	case KEY_UP:
	  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);
	  break;
	case KEY_LEFT:
	  form_driver (my_form_makbuz, REQ_LEFT_CHAR);
	  break;
	case KEY_RIGHT:
	  form_driver (my_form_makbuz, REQ_RIGHT_CHAR);
	  break;

	case KEY_F (2):	/*makbuz kay�t */

	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_PREV_FIELD);

	  if (strlen (g_strstrip (field_buffer (field[4], 0) )) < 2)
	    {
	      mesaj ( _("Makbuz no yazmad�n�z.!!!") );
	      break;
	    }

	  if (strlen (g_strstrip (field_buffer (field[5], 0))) < 2)
	    {
	      mesaj ( _("Firma ad� girmek zorundas�n�z.!!!") );
	      break;
	    }

	  if (strlen (g_strstrip (field_buffer (field[7], 0))) < 2)
	    {
	      mesaj ( _("Tutar girmediniz.!!!") );
	      break;
	    }

	  /*mysql y�l ay gun al�yor */
	  sprintf (tarih, "%s.%s.%s",
		g_strstrip( field_buffer (field[3], 0) ),
		g_strstrip( field_buffer (field[2], 0) ), 
		g_strstrip( field_buffer (field[1], 0) )
		);

	  if (tahsil == 1)
	    strcpy (tur, "tahsil");
	  else
	    strcpy (tur, "tediye");
	  
	  sprintf (query,
		   "insert into makbuz ( belge_no, tarih, firma_adi, aciklama, tutar, tur) values ('%s','%s','%s','%s', '%s', '%s')",
		   /* tarih */
		   g_strstrip( field_buffer (field[4], 0) ),
		   /*tarih, firma_adi */
		   tarih, g_strstrip( field_buffer (field[5], 0) ),
		   /*aciklama, tutar */
		   g_strstrip( field_buffer (field[6], 0) ),  
		   paradan_yaziya_cevir(g_strstrip( field_buffer (field[7], 0) )),
		   /*tur */
		   tur);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
	  /*makbuz musteri cari  kart�na i�lenecektir */
	  if (strstr (tur, "tahsil"))
	    cari_miktar_isle (g_strstrip( field_buffer (field[5], 0)),
			      atoll (g_strstrip(field_buffer (field[7], 0)) ), 1);

	  if (strstr (tur, "tediye"))
	    cari_miktar_isle (g_strstrip (field_buffer (field[5], 0)),
			      atoll (g_strstrip( field_buffer (field[7], 0))), -1);

	  donen_deger = onayla (yeni_kayit_istiyormusunuz);
	  if (donen_deger == 0) return;
	  if (donen_deger == 1)
	    {

	      attron (COLOR_PAIR (1));
	      mvprintw (1, 0,
			_("                            Yeni Tahsilat Makbuzu                               ") );
	      attroff (COLOR_PAIR (1));
	      tahsil = 1;

	      //ilk ac�l�nca en son kay�t uzerine konumlan
	      row_no_buyuk ("makbuz");
	      set_field_buffer (field[0], 0, rowno);

	      /*tarih i�ine degerler yaz�l�yor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

	      set_field_buffer (field[4], 0, "");
	      set_field_buffer (field[5], 0, "");
	      set_field_buffer (field[6], 0, "");
	      set_field_buffer (field[7], 0, "");

	      refresh ();

	    }

	  /*f2 kay�t i�lemlerinin sonu */
	  break;

	case KEY_F (3):	/*yeni kay�t */
	  donen_deger = onayla (yeni_kayit);
	  if (donen_deger == 1)
	    {
	      attron (COLOR_PAIR (1));
	      mvprintw (1, 0,
			_("                            Yeni Tahsilat Makbuzu                               ") );
	      attroff (COLOR_PAIR (1));
	      tahsil = 1;

	      //ilk ac�l�nca en son kay�t uzerine konumlan
	      row_no_buyuk ("makbuz");
	      set_field_buffer (field[0], 0, rowno);

	      /*tarih i�ine degerler yaz�l�yor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

	      set_field_buffer (field[4], 0, "");
	      set_field_buffer (field[5], 0, "");
	      set_field_buffer (field[6], 0, "");
	      set_field_buffer (field[7], 0, "");

		//en buyuk makbuz degeri okunup bir atr�l�p yazacak
		strcpy(query, "select max(belge_no) from makbuz");
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		sprintf(max_makbuz_no_str,"%s",pgsql_kayit_field());
		max_makbuz_no=atoi(max_makbuz_no_str);
		sprintf(max_makbuz_no_str,"%i",max_makbuz_no+1);
		set_field_buffer(field[4],0,max_makbuz_no_str);			

		  refresh ();

	    }

	  break;

	case KEY_F (8):	/*firma se�imi */

	  secilen = field_goster ("firmalar", "firma_adi", 0);
	  set_field_buffer (field[5], 0, secilen);
	  /*a�a��dakileri yap�nca refresh yap�yor */
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);

	  break;

	case KEY_F (7):	/*tahsil tediye */

	  if (tahsil == 1)
	    {
	      attron (COLOR_PAIR (1));
	      mvprintw (1, 0,
			_("                              Yeni Tediye Makbuzu                               ") );
	      attroff (COLOR_PAIR (1));
	      tahsil = 0;
	      refresh ();
	      break;
	    }

	  attron (COLOR_PAIR (1));
	  mvprintw (1, 0,
		    _("                            Yeni Tahsilat Makbuzu                               ") );
	  attroff (COLOR_PAIR (1));
	  tahsil = 1;

	  refresh ();
	  break;


	//makbuz bos sayfaya yazd�r
	case KEY_F (5):

		set_field_buffer( field[7],0, paradan_yaziya_cevir( g_strstrip( field_buffer(field[7], 0)  )  )  );

		form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		form_driver (my_form_makbuz, REQ_PREV_FIELD);

		if (tahsil==0)	sprintf( baslik_str, _("                                                      Tediye Makbuzu") );
		if (tahsil==1)	sprintf( baslik_str, _("                                                      Tahsilat Makbuzu" ) );
				
		//firma bilgiler al�nacak
		sprintf(query,"select sirket_adi, adres, vergi_dairesi, vergi_no, telefon1 from sirket");
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		sprintf( sirket_adi_str, "     %s", pgsql_kayit_field() );
		sprintf( adres_str, "     %s\n", pgsql_kayit_field() );
		sprintf( vergi_dairesi_str, "     %s", pgsql_kayit_field() );
		sprintf( vergi_no_str, _("  Vergi no: %s\n"), pgsql_kayit_field() );
		sprintf( telefon1_str, _("     Tel: %s\n"), pgsql_kayit_field() );
		
		sprintf( tarih_str, _("                                                                                                 Tarih: %s/%s/%s\n"),
		g_strstrip(field_buffer(field[1],0)), g_strstrip(field_buffer(field[2],0)), g_strstrip(field_buffer(field[3],0)) );
		sprintf( makbuz_str, _("                                                                                                 Makbuz no: %s\n"),
		g_strstrip(field_buffer(field[4],0)) );
		
		sprintf( tutar_str, "                                                    #%s#\n",
		yazidan_paraya_cevir( paradan_yaziya_cevir(g_strstrip(field_buffer(field[7],0) )))  );

		sprintf(yazi1_str, _("             Say�n   %s     'dan\n"), g_strstrip(field_buffer(field[5],0)) );
		sprintf(yazi2_str, _("     yaln�z %s "), para_yazdir( g_strstrip(field_buffer(field[7],0) ))    ); 
		if (tahsil==0) sprintf(yazi3_str, _("tediye edilmi�tir.\n") );
		if (tahsil==1) sprintf(yazi3_str, _("tahsil edilmi�tir.\n") );

		sprintf(aciklama_str, "     %s\n", g_strstrip(field_buffer(field[6],0) ) );
				
		yazici=fopen("/opt/acikisletme/conf/makbuz","w");
				
		tarih_ayarla();
				
		fputs("\n",yazici);
		fputs("\n",yazici);

		fputs(baslik_str ,yazici);
		fputs("\n",yazici);
		fputs("\n",yazici);
		
		fputs(sirket_adi_str ,yazici);
		fputs("\n",yazici);
		fputs(adres_str,yazici);
		fputs(vergi_dairesi_str ,yazici);
		fputs(vergi_no_str ,yazici);
		fputs(telefon1_str ,yazici);
		
		fputs( tarih_str, yazici);
		fputs( makbuz_str, yazici);
		
		fputs(tutar_str ,yazici);
		fputs("\n",yazici);
		
		fputs( yazi1_str, yazici);
		fputs( yazi2_str, yazici);	
		fputs( yazi3_str, yazici);
		
		fputs("\n",yazici);

		fputs( _("     A��klama :\n") ,yazici);
		fputs(aciklama_str ,yazici);
		
		fputs( _("                                                                                                 Yetkili imza\n") ,yazici);
			
		fputc(12,yazici);// sayfa sonu
		fclose( yazici );
		
		system("enscript --quiet --encoding=88599 --font=Tahoma12 /opt/acikisletme/conf/makbuz -B");
		system("rm -rf /opt/acikisletme/conf/makbuz");
		
		mesaj( _("Makbuz yaz�c�ya g�nderildi") );
		form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		form_driver (my_form_makbuz, REQ_PREV_FIELD);
		refresh();

	break;

	default:
	  form_driver (my_form_makbuz, ch);
	  break;

	}

    }

for (i = 0; i < 9; i++);  free_field (field[i]);
return;
}

/*******************************************************************/

int tahsil_tediye_makbuz_eski (unsigned long int kayit_no)
{
  char query[2000] = "";
  char gecici_yazi[1000] = "";
  
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

  char tur_str[20];
  char firma_adi_str[100];
  char tutar_str[30];

  touchwin (ana_win);
  wrefresh (ana_win);

  ekran_temizle();

  row_no_buyuk ("makbuz");
  if (row_no < kayit_no) kayit_no = kayit_no - 15;

sprintf (query,
"select row_no, tarih, firma_adi, tutar, tur from makbuz order by tarih limit 15 offset %d",
kayit_no);

if (strlen (g_strstrip (giris)) > 2)
sprintf (query,
"select row_no, tarih, firma_adi, tutar, tur from makbuz where firma_adi like '%%%s%%' order by tarih limit 15 offset 0",
giris);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
  
for (i=0;i<pgsql_row_miktar;i++)
    {
      sprintf (deger[i], "%-10.10s", pgsql_kayit_field() );

      sprintf (deger2[i], "%10.10s", pgsql_kayit_field() );
	  sprintf (deger2[i], "%s - %10.10s - ", deger2[i], pgsql_kayit_field() );      	  
	  strcat (deger2[i], g_strstrip(yazidan_paraya_cevir ( pgsql_kayit_field())) );
	  strcat  (deger2[i],  g_strstrip(pgsql_kayit_field() ) );
	  
      sprintf(deger2[i], "%-45.45s", deger2[i]);

      my_items[i] = new_item (&deger[i], &deger2[i]);
    }

  my_items[i] = (ITEM *) NULL;
  my_menu = new_menu ((ITEM **) my_items);

  /*3 sat�r 3 sutun 15 sat�r 60 sutun pencere */
  my_menu_win = newwin (21, 75, 1, 3);

  keypad (my_menu_win, TRUE);

  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 15, 70, 4, 1));
  set_menu_format (my_menu, 15, 1);

  set_menu_mark (my_menu, mark);

  box (my_menu_win, 0, 0);

  print_in_middle (my_menu_win, 1, 0, 75,
		   _("  F4-> D�zelt  F5-> Ara  F8-> Sil  ESC-> ��k��  Se�iminizi Yap�n�z "),
		   COLOR_PAIR (1));

  print_in_middle (my_menu_win, 20, 0, 75,
		   _("  PAGE UP -> �nceki Sayfa     PAGE DOWN -> Sonraki Sayfa "),
		   COLOR_PAIR (1));

  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, 80);
  mvwaddch (my_menu_win, 2, 79, ACS_RTEE);

  post_menu (my_menu);
  wrefresh (my_menu_win);

/*buras� �al��maz ise hata veriyor ve de�eri okuyam�yor*/
  menu_driver (my_menu, REQ_DOWN_ITEM);
  cur_item = current_item (my_menu);
  wrefresh (my_menu_win);

  while ((ch != 27))
    {
      ch = getch ();
      switch (ch)
	{
	case KEY_F (10):
	  ch = 27;
	  break;
	case KEY_DOWN:
	  menu_driver (my_menu, REQ_DOWN_ITEM);
	  break;
	case KEY_UP:
	  menu_driver (my_menu, REQ_UP_ITEM);
	  break;
	case KEY_F (4):	/* F4 Stok kartonu duzenleme ekran� */
	  sprintf (gecici_yazi, "%s", item_name (cur_item));
	  tahsil_tediye_makbuz_duzenle (gecici_yazi);
	  return;

	  break;
	case KEY_F (5):	/*arama tusu bas�l�rsa */

	  aranacak_kelime
	    ( _("�irket ad� veya yak�n bir de�er giriniz. D�zg�n s�ralama i�in bo� b�rak�n�z. ") );
	  tahsil_tediye_makbuz_eski (0);
	  return;

	  break;

	case KEY_F (8):	/*sil tusu bas�l�rsa f8 */

	  //silme hakk� kontrolu
	  if (haklari_kontrol_et (3) == 0)
	    {
	      mesaj ( _("Silme hakk�n�z yok.!!!") );
	      break;
	    }

	  donen_deger = onayla (kayit_silme_onayla);
	  if (donen_deger == 1)
	    {
		//makbuzlar tablosundan gerekli bilgiller �elkilecek ve eksiltme veya art�rma yap�lacak

	      sprintf (query, "select tur, firma_adi, tutar from makbuz where row_no=%s",
		       item_name (cur_item));
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

		sprintf(tur_str, "%s", g_strstrip(pgsql_kayit_field()));
		sprintf(firma_adi_str, "%s", g_strstrip(pgsql_kayit_field()) );
		sprintf(tutar_str, "%s", g_strstrip(pgsql_kayit_field()) );

		if (strstr(tur_str, _("tahsil") ) )
		{
		//iptal edilisrse borcuna yaz -1
		cari_miktar_isle (firma_adi_str, atoll(tutar_str), -1);
		}

		if (strstr(tur_str, _("tediye") ) )
		{
		//iptal edilisrse alacag�na yaz 1
		cari_miktar_isle (firma_adi_str, atoll(tutar_str), 1);		
		}

	    /*Silme i�lemi onayland� ilgili olan row_no bulunacak ve kay�t silinecek daha
	         sonra menu refresh edilmelidir */
	      sprintf (query, "delete from makbuz where row_no=%s",
		       item_name (cur_item));

		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
	     for (c = 0; c < i; ++c) free_item (my_items[c]);

	      tahsil_tediye_makbuz_eski (kayit_no);
	      return;
	    }
	  break;
/*page up page down yaz�lacak*/

	case KEY_NPAGE:	/* Page down bas�l�rsa */
	  	kayit_no = kayit_no + 15;
		for (c = 0; c < i; ++c)  free_item (my_items[c]);
		tahsil_tediye_makbuz_eski (kayit_no);
	  	return;
	  	break;
	case KEY_PPAGE:	/* Page up bas�l�rsa */
	  	if (kayit_no > 14)  {kayit_no = kayit_no - 15;}
	  	for (c = 0; c < i; ++c)  free_item (my_items[c]);
	  	tahsil_tediye_makbuz_eski (kayit_no);
		return;
	  break;
	}
      cur_item = current_item (my_menu);
      wrefresh (my_menu_win);
    }

  /* uzerinde bulunan menu degerinini isim degeri yani ilk deger rowno */
  //sprintf(donen,"%s",item_name(cur_item));

  return;
}


/******************************************************************************/

int tahsil_tediye_makbuz_duzenle (unsigned long int kayit_no)
{
/*bu fonksiyon stok_karton_eski taraf�ndan menude duzenle t�klan�nca �a�r�l�r
gonderilen rowno ile duzenleme ekran� a��l�r*/

char query[2000] = "";
char tarih[10] = "";

FIELD *field[9];
FORM *my_form;
int ch, i;

int tahsil = 1;

char tur[6] = "";
char para_str[30];

char baslik_str[100];
char sirket_adi_str[100];
char adres_str[100];
char vergi_dairesi_str[100];
char vergi_no_str[100];
char telefon1_str[100];
char tarih_str[100];
char makbuz_str[100];
char tutar_str[100];
char yazi1_str[100];
char yazi2_str[100];
char yazi3_str[100];
char aciklama_str[100];
char bilgi_str[200];


  touchwin (ana_win);
  wrefresh (ana_win);

  ekran_temizle();

//set_field_type(field[2], TYPE_ALNUM);
//set_field_type(field[6], TYPE_INTEGER);

/*row_no int*/
/*Kay�t i�leminden sonra row no de�eri verilir ekrana yaz�l�r*/
  field[0] = new_field (1, 8, 3, 5, 0, 0);
/*tarih date*/
  field[1] = new_field (1, 2, 3, 29, 0, 0);
  field[2] = new_field (1, 2, 3, 32, 0, 0);
  field[3] = new_field (1, 4, 3, 35, 0, 0);
/*belge_no char*/
  field[4] = new_field (1, 15, 3, 60, 0, 0);

/*firma*/
  field[5] = new_field (1, 60, 5, 15, 0, 0);

/*ac�klama*/
  field[6] = new_field (1, 60, 7, 15, 0, 0);

/*tutar*/
  field[7] = new_field (1, 25, 9, 15, 0, 0);
  field[8] = NULL;

//tarih alan�na int girmek zorundalar
  set_field_type (field[1], TYPE_INTEGER);
  set_field_type (field[2], TYPE_INTEGER);
  set_field_type (field[3], TYPE_INTEGER);
      
  set_field_just (field[0], JUSTIFY_RIGHT);
  set_field_just (field[5], JUSTIFY_LEFT);
  set_field_just (field[6], JUSTIFY_LEFT);
  set_field_just (field[7], JUSTIFY_RIGHT);

/*yaz� yaz�lamaz sadece gorme modunda ilk ac�l�nca*/
  field_opts_off (field[0], O_ACTIVE);
/*firmay aelle yaz� yaz�lmaz*/
  field_opts_off (field[5], O_EDIT);

  my_form = new_form (field);
  post_form (my_form);

  attron (COLOR_PAIR (1));
  mvprintw (1, 0,
	    _("                            Yeni Tahsilat Makbuzu                               ") );
  attroff (COLOR_PAIR (1));

  attron (COLOR_PAIR (2));
  baslik_goruntule ();
  attroff (COLOR_PAIR (2));


for (i=0;i<8;i++)
{
set_field_fore(field[i], COLOR_PAIR(2));
set_field_back(field[i], COLOR_PAIR(2));
}

/*toplam 3 sat�r*/
  mvprintw (3, 1, _("No :") );
  mvprintw (3, 20, _("Tarih :") );
  mvprintw (3, 31, _("/") );
  mvprintw (3, 34, _("/") );
  mvprintw (3, 45, _("Makbuz No :") );

  mvprintw (5, 1, _("Firma   :") );

  mvprintw (7, 1, _("A��klama:") );
  mvprintw (9, 1, _("Tutar   :") );

mvprintw(LINES - 5, 0, _("    F5->Makbuz yazd�r") );
mvprintw(LINES - 3, 0, _("    F2->Kaydet   F3->Yeni  F8->Firma Se�imi   F7->Tahsil/Tediye   ESC->��k��") );

/*kayit_no bilgiisi row_no bilgisidiri bu bilgi ile sql den bilgi �ekilecek ve 
field lar i�ine yerle�tirilecektir*/

sprintf (query,
"select row_no, tarih, belge_no, firma_adi, aciklama, tutar, tur from makbuz where row_no=%s",
kayit_no);
  
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

for (i=0;i<pgsql_row_miktar;i++)
    {
      //row_no
      set_field_buffer (field[0], 0, pgsql_kayit_field() );

      sql_tarih_ayir ( pgsql_kayit_field() );
      //tarih
      set_field_buffer (field[1], 0, sql_gun);
      set_field_buffer (field[2], 0, sql_ay);
      set_field_buffer (field[3], 0, sql_yil);

      //belge_no
      set_field_buffer (field[4], 0, pgsql_kayit_field() );
      //firma_adi
      set_field_buffer (field[5], 0, pgsql_kayit_field() );
      //aciklama              
      set_field_buffer (field[6], 0, pgsql_kayit_field() );
      //tutar
		
      set_field_buffer (field[7], 0, yazidan_paraya_cevir(pgsql_kayit_field()) );

      strcpy (tur, pgsql_kayit_field() );
    }

  if (strstr (tur, "tahsil"))
    {
      attron (COLOR_PAIR (1));
      mvprintw (1, 0,
		_("                            Tahsilat Makbuzu D�zeltme                           ") );
      attroff (COLOR_PAIR (1));
      tahsil = 1;
    }

  if (strstr (tur, "tediye"))
    {
      attron (COLOR_PAIR (1));
      mvprintw (1, 0,
		_("                            Tediye Makbuzu D�zeltme                             ") );
      attroff (COLOR_PAIR (1));
      tahsil = 0;
    }

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
	case 8:		/*KEY_BACKSPACE hatal� kod vermekte 8 dogru oland�r */
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case KEY_BACKSPACE:
	  form_driver (my_form, REQ_PREV_CHAR);
	  form_driver (my_form, REQ_DEL_CHAR);
	  break;
	case 9:		/*TAB tusuna bas�lmas� durumunda sonraki field konumlan */
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case 10:		/*ENTER tu�una bas�lmas� durumunda sonraki feild gececek */
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);	  
	  //para format�na cevrilecek	  	  
	  sprintf(para_str,"%s", paradan_yaziya_cevir(g_strstrip(field_buffer(field[7], 0)))  );
	  set_field_buffer(field[7], 0,  yazidan_paraya_cevir(para_str) );
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
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_UP:
	  form_driver (my_form, REQ_PREV_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_LEFT:
	  form_driver (my_form, REQ_LEFT_CHAR);
	  break;
	case KEY_RIGHT:
	  form_driver (my_form, REQ_RIGHT_CHAR);
	  break;
	case KEY_F (2):	//duzeltme i�leminde kay�t
	  /*Kay�t yapmadan evvel sonraki alana gecilmeli okuma yapm�yor aksi halde */
	  form_driver (my_form, REQ_NEXT_FIELD);
	  /*y�l ay gun al�yor */
	  sprintf (tarih, "%s.%s.%s", g_strstrip(field_buffer (field[3], 0)),
		   g_strstrip(field_buffer (field[2], 0)),  g_strstrip(field_buffer (field[1], 0))  );
	  if (tahsil == 1)    strcpy (tur, "tahsil");
	  else   strcpy (tur, "tediye");
	  /* update i�lemi yap�l�yor */
	  sprintf (query,
      "update makbuz set belge_no='%s', tarih='%s', firma_adi='%s', aciklama='%s', tutar='%s', tur='%s'  where (row_no=%s) ",
	  /*belge_no */
	  g_strstrip(field_buffer (field[4], 0)),
	  /*tarih, firma_adi */
		   tarih, g_strstrip(field_buffer (field[5], 0)),
		   /*aciklama, tutar */
		   g_strstrip(field_buffer (field[6], 0)), paradan_yaziya_cevir(g_strstrip(field_buffer (field[7], 0) ) ),
		   /*tur   sorgulama sekli */
		   tur, g_strstrip(field_buffer (field[0], 0)) );

	  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	  mesaj (duzenleme_basarili);
	  for (i = 0; i < 9; i++) free_field (field[i]);
	  tahsil_tediye_makbuz_eski (0);
	  return;
	  break;
	  /* duzeltme i�lemlerinin sonu */
	case KEY_F (7):	/*firma se�imi */
	  secilen = field_goster ("firmalar", "firma_adi", 0);
	  set_field_buffer (field[5], 0, secilen);
	  /*a�a��dakileri yap�nca refresh yap�yor */
	  form_driver (my_form, REQ_NEXT_FIELD);
	  form_driver (my_form, REQ_END_LINE);
	  break;
	case KEY_F (8):	/*tahsil tediye */
	  if (tahsil == 1)
	    {
	      attron (COLOR_PAIR (1));
	      mvprintw (1, 0,
			_("                              Tediye Makbuzu D�zeltme                           ") );
	      attroff (COLOR_PAIR (1));
	      tahsil = 0;
	      refresh ();
	      break;
	    }

	  	attron (COLOR_PAIR (1));
	  	mvprintw (1, 0,
		   _("                            Tahsilat Makbuzu D�zeltme                           ") );
	  	attroff (COLOR_PAIR (1));
	  	tahsil = 1;
	  	refresh ();
	  	break;
		//makbuz bos sayfaya yazd�r
	case KEY_F (5):	
		set_field_buffer( field[7],0, paradan_yaziya_cevir( g_strstrip( field_buffer(field[7], 0)  )  )  );

		form_driver (my_form, REQ_NEXT_FIELD);
		form_driver (my_form, REQ_PREV_FIELD);

		if (tahsil==0)	sprintf( baslik_str, "                                                      Tediye Makbuzu");
		if (tahsil==1)	sprintf( baslik_str, "                                                      Tahsilat Makbuzu" );
				
		//firma bilgiler al�nacak
		sprintf(query,"select sirket_adi, adres, vergi_dairesi, vergi_no, telefon1 from sirket");
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		sprintf( sirket_adi_str, "     %s", pgsql_kayit_field() );
		sprintf( adres_str, "     %s\n", pgsql_kayit_field() );
		sprintf( vergi_dairesi_str, "     %s", pgsql_kayit_field() );
		sprintf( vergi_no_str, _("  Vergi no: %s\n"), pgsql_kayit_field() );
		sprintf( telefon1_str, _("     Tel: %s\n"), pgsql_kayit_field() );
		
		sprintf( tarih_str, _("                                                                                                 Tarih: %s/%s/%s\n"),
		g_strstrip(field_buffer(field[1],0)), g_strstrip(field_buffer(field[2],0)), g_strstrip(field_buffer(field[3],0)) );
		sprintf( makbuz_str, _("                                                                                                 Makbuz no: %s\n"),
		g_strstrip(field_buffer(field[4],0)) );
		
		sprintf( tutar_str, "                                                    #%s#\n",
		yazidan_paraya_cevir( paradan_yaziya_cevir(g_strstrip(field_buffer(field[7],0) )))  );

		sprintf(yazi1_str, _("             Say�n   %s     'dan\n"), g_strstrip(field_buffer(field[5],0)) );
		sprintf(yazi2_str, _("     yaln�z %s "), para_yazdir( g_strstrip(field_buffer(field[7],0) ))    ); 
		if (tahsil==0) sprintf(yazi3_str, _("tediye edilmi�tir.\n") );
		if (tahsil==1) sprintf(yazi3_str, _("tahsil edilmi�tir.\n") );

		sprintf(aciklama_str, "     %s\n", g_strstrip(field_buffer(field[6],0) ) );
				
		yazici=fopen("/opt/acikisletme/conf/makbuz","w");
				
		tarih_ayarla();
				
		fputs("\n",yazici);
		fputs("\n",yazici);

		fputs(baslik_str ,yazici);
		fputs("\n",yazici);
		fputs("\n",yazici);
		
		fputs(sirket_adi_str ,yazici);
		fputs("\n",yazici);
		fputs(adres_str,yazici);
		fputs(vergi_dairesi_str ,yazici);
		fputs(vergi_no_str ,yazici);
		fputs(telefon1_str ,yazici);
		
		fputs( tarih_str, yazici);
		fputs( makbuz_str, yazici);
		
		fputs(tutar_str ,yazici);
		fputs("\n",yazici);
		
		fputs( yazi1_str, yazici);
		fputs( yazi2_str, yazici);	
		fputs( yazi3_str, yazici);
		
		fputs("\n",yazici);

		fputs( _("     A��klama :\n") ,yazici);
		fputs(aciklama_str ,yazici);
		
		fputs( _("                                                                                                 Yetkili imza\n") ,yazici);
			
		fputc(12,yazici);// sayfa sonu
		fclose( yazici );
		
		system("enscript --quiet --encoding=88599 --font=Tahoma12 /opt/acikisletme/conf/makbuz -B");
		system("rm -rf /opt/acikisletme/conf/makbuz");
		
		mesaj( _("Makbuz yaz�c�ya g�nderildi") );
		form_driver (my_form, REQ_NEXT_FIELD);
		form_driver (my_form, REQ_PREV_FIELD);
		refresh();
	

	break;


	default:
	  form_driver (my_form, ch);
	  break;
	  
	}

    }

for (i = 0; i < 9; i++);  free_field (field[i]);

}

/*********************************************************/

