
int ceksenet ()
{
  ITEM **my_items;
  int ch, c;
  MENU *cari_menu;
  WINDOW *cari_win;
  int n_choices, i;
  ITEM *cur_item;

  char *choices[] = {
    _("Cheque Incoming"),
    _("Cheque Outgoing"),
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
  print_in_middle (cari_win, 1, 0, 45, _("Cheq List"), COLOR_PAIR (1));
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
	    case 1:	
			
	      ceksenet_giris ();

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;
		
	    case 2:

	      ceksenet_eski (0);

	      baslik_goruntule ();
	      baslik_yaz();
	      touchwin (cari_win);
	      wrefresh (cari_win);
	      break;

	    case 3:	

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

/*esc basýlýnca kapatýlýr*/

 
  for (i = 0; i < n_choices; ++i) free_item (my_items[i]);

  delwin (cari_win);

  return 0;
}



/******************************************************************************/

int ceksenet_giris ()
{

WINDOW *makbuz_win;
char query[2000] = "";
char tarih[10] = "";
char cektarih[10] = "";
	
FIELD *field[15];	
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
//makbuz yardýrma deðiþkenleri
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
/*Kayýt iþleminden sonra row no deðeri verilir ekrana yazýlýr*/
  field[0] = new_field (1, 8, 3, 5, 0, 0);
/*tarih date*/
  field[1] = new_field (1, 2, 3, 29, 0, 0);
  field[2] = new_field (1, 2, 3, 32, 0, 0);
  field[3] = new_field (1, 4, 3, 35, 0, 0);
/*belge_no char*/
  field[4] = new_field (1, 15, 3, 60, 0, 0);

/*firma*/
  field[5] = new_field (1, 60, 5, 15, 0, 0);

/*acýklama*/
  field[6] = new_field (1, 60, 7, 15, 0, 0);

/*tutar*/
  field[7] = new_field (1, 25, 9, 15, 0, 0);
  
/*banka adý*/  
field[8] = new_field (1, 25, 11, 15, 0, 0);  
  
/*cek no*/  
  field[9] = new_field (1, 15, 11, 60, 0, 0);
  
/*cek tarihi */
  field[10] = new_field (1, 2, 13, 19, 0, 0);
  field[11] = new_field (1, 2, 13, 22, 0, 0);
  field[12] = new_field (1, 4, 13, 25, 0, 0);
  
  /*keside yeri*/
  field[13] = new_field (1, 15, 13, 60, 0, 0);
  
  /*ceki kesen firma*/
  field[14] = new_field (1, 30, 15, 45, 0, 0);
    
  field[15] = NULL;

//tarih alanýna int girmek zorundalar
/*
  set_field_type (field[1], TYPE_INTEGER);
  set_field_type (field[2], TYPE_INTEGER);
  set_field_type (field[3], TYPE_INTEGER);

  set_field_type (field[10], TYPE_INTEGER);
  set_field_type (field[11], TYPE_INTEGER);
  set_field_type (field[12], TYPE_INTEGER);
*/
//  set_field_type (field[7], TYPE_NUMERIC);

  set_field_just (field[0], JUSTIFY_RIGHT);
  set_field_just (field[5], JUSTIFY_LEFT);
  set_field_just (field[6], JUSTIFY_LEFT);
  set_field_just (field[7], JUSTIFY_RIGHT);

/*yazý yazýlamaz sadece gorme modunda ilk acýlýnca*/
  field_opts_off (field[0], O_ACTIVE);
/*firmay aelle yazý yazýlmaz*/
  field_opts_off (field[5], O_EDIT);

  my_form_makbuz = new_form (field);

//  set_form_win (my_form_makbuz, makbuz_win);
//  set_form_sub (my_form_makbuz, derwin (makbuz_win, 25, 80, 0, 0));
  post_form (my_form_makbuz);


  attron (COLOR_PAIR (2));
  mvprintw (1, 0,
  _("                              Çek Tahsilat Ekraný                               ") );
  attroff (COLOR_PAIR (2));

 //baslik_goruntule ();
  
for (i=1;i<15;i++)
{
set_field_fore(field[i], COLOR_PAIR(2));
set_field_back(field[i], COLOR_PAIR(2));
}

	mvprintw (3, 1, _("No :") );
	mvprintw (3, 20, _("Tarih :") );
	attron (COLOR_PAIR (2));
	mvprintw (3, 31, "/");
	mvprintw (3, 34, "/");
	attroff (COLOR_PAIR (2));
	mvprintw (3, 45, _("Makbuz No :") );
	mvprintw (5, 1, _("Gelen Firma:") );
	mvprintw (7, 1, _("Açýklama   :") );
	mvprintw (9, 1, _("Çek Tutarý :") );

	mvprintw (11, 1, _("Banka Adý  :") );
	mvprintw (11, 45, _("Çek No  :") );

	mvprintw (13, 1, _("Çek Tarihi :") );
	attron (COLOR_PAIR (2));  
  	mvprintw (13, 21, "/");
  	mvprintw (13, 24, "/");
	attroff (COLOR_PAIR (2));

  mvprintw (13, 45, _("Keþide Yeri :") );
  mvprintw (15, 1, _("Çeki Kesen Þahýs veya Firma Adý   :") );
  mvprintw(LINES - 2, 0, _("    F2->Kaydet   F3->Yeni  F8->Firma Seçimi   F5->Makbuz yazdýr   ESC->Çýkýþ") );

/*tarih içine degerler yazýlýyor*/
/*gun ay yil degerleri yaziliyor*/
  tarih_ayarla ();

  set_field_buffer (field[1], 0, gun);
  set_field_buffer (field[2], 0, ay);
  set_field_buffer (field[3], 0, yil);

 //en buyuk makbuz degeri okunup bir atrýlýp yazacak
  strcpy(query, "select max(belge_no) from ceksenet");
  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
  sprintf(max_makbuz_no_str,"%s",pgsql_kayit_field());
  max_makbuz_no=atoi(max_makbuz_no_str);
  sprintf(max_makbuz_no_str,"%i",max_makbuz_no+1);
  set_field_buffer(field[4],0,max_makbuz_no_str);			

//ilk acýlýnca en son kayýt uzerine konumlan
  row_no_buyuk ("ceksenet");
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
	case 8:	/*KEY_BACKSPACE hatalý kod vermekte 8 dogru olandýr konsolda 8 xterm key_backspace */
	  form_driver (my_form_makbuz, REQ_PREV_CHAR);
	  form_driver (my_form_makbuz, REQ_DEL_CHAR);
	  break;
	case KEY_BACKSPACE:
	  form_driver (my_form_makbuz, REQ_PREV_CHAR);
	  form_driver (my_form_makbuz, REQ_DEL_CHAR);
	  break;	
	case 9:	/*TAB tusuna basýlmasý durumunda sonraki field konumlan */
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);
	  break;
	case 10:/*ENTER tuþuna basýlmasý durumunda sonraki feild gececek */
 	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);	  
	  //para formatýna cevrilecek	  	  
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
	case KEY_F (2):	/*makbuz kayýt */
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	  if (strlen (g_strstrip (field_buffer (field[4], 0) )) < 2)
	    {
	      mesaj ( _("Makbuz no yazmadýnýz.!!!") );
		  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	      break;
	    }

	  if (strlen (g_strstrip (field_buffer (field[5], 0))) < 2)
	    {
	      mesaj ( _("Firma adý girmek zorundasýnýz.!!!") );
		  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	      break;
	    }

	  if (strlen (g_strstrip (field_buffer (field[7], 0))) < 2)
	    {
	      mesaj ( _("Tutar girmediniz.!!!") );
		  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	      break;
	    }
		
	  if (strlen (g_strstrip (field_buffer (field[10], 0))) < 2)
	    {
	      mesaj ( _("Çek tarihi hatalý.!!!") );
		  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	      break;
	    }
	  		
		
	  if (strlen (g_strstrip (field_buffer (field[11], 0))) < 2)
	    {
	      mesaj ( _("Cek tarihi hatalý.!!!") );
		  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	      break;
	    }	  		
		
	  if (strlen (g_strstrip (field_buffer (field[12], 0))) < 2)
	    {
	      mesaj ( _("Cek tarihi hatalý.!!!") );
		  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		  form_driver (my_form_makbuz, REQ_PREV_FIELD);
	      break;
	    }
	  		
		
	  sprintf (tarih, "%s.%s.%s",
		g_strstrip( field_buffer (field[3], 0) ),
		g_strstrip( field_buffer (field[2], 0) ), 
		g_strstrip( field_buffer (field[1], 0) )
		);

	  sprintf (cektarih, "%s.%s.%s",
		g_strstrip( field_buffer (field[10], 0) ),
		g_strstrip( field_buffer (field[11], 0) ), 
		g_strstrip( field_buffer (field[12], 0) )
		);

	  
	  sprintf (query,
"insert into ceksenet ( belge_no, islem_tarih, firma_adi, aciklama, tutar, banka_adi, cek_no, cek_tarih, keside_yer, cek_kesen_firma, durum) values ('%s', '%s', '%s', '%s', '%s', '%s','%s','%s','%s', '%s', '%s')",
		   /* makbuzno */
		   g_strstrip( field_buffer (field[4], 0) ),
		   /*tarih, firma_adi */
		   tarih, g_strstrip( field_buffer (field[5], 0) ),
		   /*aciklama, tutar */
		   g_strstrip( field_buffer (field[6], 0) ),  
		   paradan_yaziya_cevir(g_strstrip( field_buffer (field[7], 0) )),
		   //banka
		   g_strstrip( field_buffer (field[8], 0) ),  
		   //cekno
		   g_strstrip( field_buffer (field[9], 0) ),  
		   //cek tarihi
		   cektarih, 
		   //keside yer		   
		   g_strstrip( field_buffer (field[10], 0) ),  
		   //cek kesen firma
		   g_strstrip( field_buffer (field[11], 0) ),
		   //durumu nedir  onemli   -- sirket : elimzide olanlar  -- banka:tahsil edilenler  -- firma:musteriye cýkanlar
		   //su anda elimize yeni geldigi icin sirket yazýlacak
		   "sirket"
		   );

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
	  /*makbuz musteri cari  kartýna iþlenecektir */
	  cari_miktar_isle (g_strstrip( field_buffer (field[5], 0)),
			      atoll (g_strstrip(field_buffer (field[7], 0)) ), 1);
	  
	  donen_deger = onayla (yeni_kayit_istiyormusunuz);
		form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		form_driver (my_form_makbuz, REQ_PREV_FIELD);
	  if (donen_deger == 0) return;
	  if (donen_deger == 1)
	    {
		    attron (COLOR_PAIR (2));
  			mvprintw (1, 0,
  			_("                              Çek Tahsilat Ekraný                               ") );
  			attroff (COLOR_PAIR (2));

	      //ilk acýlýnca en son kayýt uzerine konumlan
	      row_no_buyuk ("ceksenet");
	      set_field_buffer (field[0], 0, rowno);

	      /*tarih içine degerler yazýlýyor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

		for (i=4;i<15;i++) set_field_buffer (field[i], 0, "");
	      
	      refresh ();

	    }

	  /*f2 kayýt iþlemlerinin sonu */

	  break;

	case KEY_F (3):	/*yeni kayýt */

	  donen_deger = onayla (yeni_kayit);
		
	form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	form_driver (my_form_makbuz, REQ_PREV_FIELD);
	
	  if (donen_deger == 1)
	    {

		attron (COLOR_PAIR (2));
  		mvprintw (1, 0,
		_("                              Çek Tahsilat Ekraný                               ") );
  		attroff (COLOR_PAIR (2));
			
	      //ilk acýlýnca en son kayýt uzerine konumlan
	      row_no_buyuk ("ceksenet");
	      set_field_buffer (field[0], 0, rowno);

	      /*tarih içine degerler yazýlýyor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

		for (i=4;i<15;i++) set_field_buffer (field[i], 0, "");
			
		//en buyuk makbuz degeri okunup bir atrýlýp yazacak
		strcpy(query, "select max(belge_no) from ceksenet");
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		sprintf(max_makbuz_no_str,"%s",pgsql_kayit_field());
		max_makbuz_no=atoi(max_makbuz_no_str);
		sprintf(max_makbuz_no_str,"%i",max_makbuz_no+1);
		set_field_buffer(field[4],0,max_makbuz_no_str);			

		refresh ();

	    }
	  break;

	case KEY_F (8):	/*firma seçimi */
	  secilen = field_goster ("firmalar", "firma_adi", 0);
	  set_field_buffer (field[5], 0, secilen);
	  /*aþaðýdakileri yapýnca refresh yapýyor */
	  form_driver (my_form_makbuz, REQ_NEXT_FIELD);
	  form_driver (my_form_makbuz, REQ_END_LINE);

	  break;

	//makbuz bos sayfaya yazdýr
	case KEY_F (5):
		set_field_buffer( field[7],0, paradan_yaziya_cevir( g_strstrip( field_buffer(field[7], 0)  )  )  );
		form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		form_driver (my_form_makbuz, REQ_PREV_FIELD);
		sprintf( baslik_str, "                                                  Çek Tahsilat Makbuzu" );
		//firma bilgiler alýnacak
		sprintf(query,"select sirket_adi, adres, vergi_dairesi, vergi_no, telefon1 from sirket");
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		sprintf( sirket_adi_str, "     %s", pgsql_kayit_field() );
		sprintf( adres_str, "     %s\n", pgsql_kayit_field() );
		sprintf( vergi_dairesi_str, "     %s", pgsql_kayit_field() );
		sprintf( vergi_no_str, "  Vergi no: %s\n", pgsql_kayit_field() );
		sprintf( telefon1_str, "     Tel: %s\n", pgsql_kayit_field() );
		
		sprintf( tarih_str, "                                                                                                 Tarih: %s/%s/%s\n",
		g_strstrip(field_buffer(field[1],0)), g_strstrip(field_buffer(field[2],0)), g_strstrip(field_buffer(field[3],0)) );
		sprintf( makbuz_str,"                                                                                                 Makbuz no: %s\n",
		g_strstrip(field_buffer(field[4],0)) );
		
		sprintf( tutar_str, "                                                    #%s#\n",
		yazidan_paraya_cevir( paradan_yaziya_cevir(g_strstrip(field_buffer(field[7],0) )))  );

		sprintf(yazi1_str, "      Sayýn   %s  'dan", g_strstrip(field_buffer(field[5],0)) );
		sprintf(yazi2_str, " yalnýz %s ", para_yazdir( g_strstrip(field_buffer(field[7],0) ))    ); 
		sprintf(yazi3_str, "tahsil edilmiþtir.\n");

		sprintf(aciklama_str, "%s\n%s\n%s.%s.%s tarihli çek ile", g_strstrip(field_buffer(field[8],0) ), 
			g_strstrip(field_buffer(field[9],0) ), g_strstrip(field_buffer(field[10],0) ),
			g_strstrip(field_buffer(field[11],0) ),	g_strstrip(field_buffer(field[12],0) )  );
				
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

		fputs( _("     Açýklama :\n") ,yazici);
		fputs(aciklama_str ,yazici);
		
		fputs( _("                                                                         Yetkili imza\n") ,yazici);
				
		fclose( yazici );
		
		system("enscript --quiet --encoding=88599 --font=Tahoma12 /opt/acikisletme/conf/makbuz -B");
		system("rm -rf /opt/acikisletme/conf/makbuz");
		
		mesaj( _("Makbuz yazýcýya gönderildi") );
		form_driver (my_form_makbuz, REQ_NEXT_FIELD);
		form_driver (my_form_makbuz, REQ_PREV_FIELD);
		refresh();

	break;

	default:
	  form_driver (my_form_makbuz, ch);
	  break;

	}

    }

for (i = 0; i < 15; i++);  free_field (field[i]);

return;

}

/*******************************************************************/

int ceksenet_eski (unsigned long int kayit_no)
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
  char cek_row_no[20];

  touchwin (ana_win);
  wrefresh (ana_win);

  ekran_temizle();

  row_no_buyuk ("ceksenet");
  if (row_no < kayit_no) kayit_no = kayit_no - 15;

sprintf (query,
"select row_no, islem_tarih, firma_adi, tutar from ceksenet where durum='sirket' order by islem_tarih limit 15 offset %d",
kayit_no);

if (strlen (g_strstrip (giris)) > 2)
sprintf (query,
"select row_no, islem_tarih, firma_adi, tutar from ceksenet where firma_adi like '%%%s%%' and durum='sirket' order by islem_tarih limit 15 offset 0",
giris);

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
  
for (i=0;i<pgsql_row_miktar;i++)
    {
      sprintf (deger[i], "%-10.10s", pgsql_kayit_field() );

      sprintf (deger2[i], "%10.10s", pgsql_kayit_field() );
	  sprintf (deger2[i], "%s - %15.15s - ", deger2[i], pgsql_kayit_field() );      	  
	  strcat (deger2[i], g_strstrip(yazidan_paraya_cevir ( pgsql_kayit_field())) );
	  //strcat  (deger2[i],  g_strstrip(pgsql_kayit_field() ) );
	  
      sprintf(deger2[i], "%-50.50s", deger2[i]);

      my_items[i] = new_item (&deger[i], &deger2[i]);
    }

  my_items[i] = (ITEM *) NULL;
  my_menu = new_menu ((ITEM **) my_items);

  /*3 satýr 3 sutun 15 satýr 60 sutun pencere */
  my_menu_win = newwin (21, 75, 1, 3);

  keypad (my_menu_win, TRUE);

  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 15, 70, 4, 1));
  set_menu_format (my_menu, 15, 1);

  set_menu_mark (my_menu, mark);

  box (my_menu_win, 0, 0);

  print_in_middle (my_menu_win, 1, 0, 75,
  _("F3->Bankaya Çýkýþ  F4->Firmaya Çýkýþ  F5->Ara  F8->Sil  ESC->Çýkýþ"),
  COLOR_PAIR (1));

  print_in_middle (my_menu_win, 20, 0, 75,
  _("  PAGE UP -> Önceki Sayfa     PAGE DOWN -> Sonraki Sayfa "),
  COLOR_PAIR (1));

  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, 80);
  mvwaddch (my_menu_win, 2, 79, ACS_RTEE);

  post_menu (my_menu);
  wrefresh (my_menu_win);

/*burasý çalýþmaz ise hata veriyor ve deðeri okuyamýyor*/
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
	
	case KEY_F (3):	/* F3  bankaya çýkýþ */

	sprintf(cek_row_no, "%s", item_name (cur_item) );
	
	  donen_deger = onayla ( _("Seçilen çekin bankadan tahsilini onaylýyormusunuz?") );
	  if (donen_deger == 1)
	    {		
	    sprintf (query, "update ceksenet set durum='banka' where row_no=%s", cek_row_no);		       			
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		/*
		//para hesaptan dusulecek
		sprintf (query, "select tutar from ceksenet where row_no=%s", cek_row_no);		       			
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
		cari_miktar_isle(firma_adi_str, atoll(pgsql_kayit_field()), -1);
		*/
		}
			
      ceksenet_eski (0);
	  return;
	  break;
	
	case KEY_F (4):	/* F4 Firmaya çýkýþ */
			
	sprintf(cek_row_no, "%s", item_name (cur_item) );
	
	  donen_deger = onayla ( _("Seçilen çekin, seçilecek olan firmaya çýkýþýný     kabul ediyormusunuz.?") );
	  if (donen_deger == 1)
	    {		
		sprintf(firma_adi_str, "%s", field_goster("firmalar" , "firma_adi", 0) );			
	    sprintf (query, "update ceksenet set cikan_firma_adi='%s', durum='firma' where row_no=%s",
		       firma_adi_str, cek_row_no);		       			
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
		//para hesaptan dusulecek
		sprintf (query, "select tutar from ceksenet where row_no=%s", cek_row_no);		       			
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
		cari_miktar_isle(firma_adi_str, atoll(pgsql_kayit_field()), -1);		
		}
	
	  ceksenet_eski (0);
	  return;

	  break;
	case KEY_F (5):	/*arama tusu basýlýrsa */

	  aranacak_kelime
	    ( _("Þirket adý veya yakýn bir deðer giriniz. Düzgün sýralama için boþ býrakýnýz. ") );
	  ceksenet_eski (0);
	  return;

	  break;

	case KEY_F (8):	/*sil tusu basýlýrsa f8 */

	  //silme hakký kontrolu
	  if (haklari_kontrol_et (3) == 0)
	    {
	      mesaj ( _("Silme hakkýnýz yok.!!!") );
	      break;
	    }

	  donen_deger = onayla (kayit_silme_onayla);
	  if (donen_deger == 1)
	    {
		//makbuzlar tablosundan gerekli bilgiller çelkilecek ve eksiltme veya artýrma yapýlacak
		sprintf (query, "select tur, firma_adi, tutar from ceksenet where row_no=%s",   item_name (cur_item));
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

		sprintf(tur_str, "%s", g_strstrip(pgsql_kayit_field()));
		sprintf(firma_adi_str, "%s", g_strstrip(pgsql_kayit_field()) );
		sprintf(tutar_str, "%s", g_strstrip(pgsql_kayit_field()) );

		//iptal edilisrse borcuna yaz -1
		cari_miktar_isle (firma_adi_str, atoll(tutar_str), -1);
		
	    /*Silme iþlemi onaylandý ilgili olan row_no bulunacak ve kayýt silinecek daha sonra menu refresh edilmelidir */
	      sprintf (query, "delete from ceksenet where row_no=%s", item_name (cur_item));

		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
	     	for (c = 0; c < i; ++c) free_item (my_items[c]);

	      	ceksenet_eski (kayit_no);
	      	return;
	    }
	  break;
/*page up page down yazýlacak*/

	case KEY_NPAGE:	/* Page down basýlýrsa */
		kayit_no = kayit_no + 15;
	  	for (c = 0; c < i; ++c)  free_item (my_items[c]);
	  	ceksenet_eski (kayit_no);
	  	return;
	  	break;

	case KEY_PPAGE:	/* Page up basýlýrsa */
	  	if (kayit_no > 14) {   kayit_no = kayit_no - 15;	    }
	  	for (c = 0; c < i; ++c)  free_item (my_items[c]);
	  	ceksenet_eski (kayit_no);
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

