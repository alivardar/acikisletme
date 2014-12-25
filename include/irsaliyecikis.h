
int irsaliye_cikis()
{

char tarih[10] = "";
char durum = 0;		//0=gezinti, 1=yeni, 2=duzeltme
FIELD *field[88];
FORM *my_form;
int ch, satir, sira;

long int rowno_miktar;
long int rownno;

long long int toplam;
long long int toplam_kdv;
long long int toplam_tutar;
long long int tutar;

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

char duzenleme_saati[15];
char sevk_tarihi[15];

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

/*talan char*/
  field[7] = new_field (1, 23, 20, 15, 0, 0);
/*teden char*/
  field[8] = new_field (1, 23, 21, 15, 0, 0);
//teslim adresi
  field[9] = new_field (1, 23, 22, 15, 0, 0);

//sevk tarihi
  field[10] = new_field (1, 20, 20, 58, 0, 0);
//duzenleme saati
  field[11] = new_field (1, 20, 21, 58, 0, 0);
//duzenleyen
  field[12] = new_field (1, 20, 22, 58, 0, 0);
			

satir = 4;
for (i = 13; i < 88; i = i + 5)
    {
//malzeme_kodu char
      field[i] = new_field (1, 16, satir, 1, 1, 0);
//urun adý
      field[i + 1] = new_field (1, 25, satir, 18, 1, 0);
//miktar
      field[i + 2] = new_field (1, 8, satir, 44, 1, 0);
//birim
      field[i + 3] = new_field (1, 6, satir, 53, 1, 0);
//birim_fiyat
      field[i + 4] = new_field (1, 19, satir, 60, 1, 0);

	  set_field_just (field[i + 2], JUSTIFY_RIGHT);
	  set_field_just (field[i + 4], JUSTIFY_RIGHT);

	  satir = satir + 1;
    }

  field[88] = NULL;

field_opts_off (field[0], O_ACTIVE);
  
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

	
  for (i = 0; i < 88; i++)
	{
    field_opts_off (field[i], O_EDIT);
	field_opts_off (field[i], O_AUTOSKIP);
	}


attron (COLOR_PAIR (2));
attron(A_BOLD);
mvprintw (0, 0,  _("                   Outgoing Waybill Screen                           ") );
attroff(A_BOLD);
attroff (COLOR_PAIR (2));

/*toplam 3 satýr*/
  mvprintw (1, 1, _("No:") );
  mvprintw (1, 15, _("Tarih:") );
  mvprintw (1, 24, "/");
  mvprintw (1, 27, "/");
  mvprintw (1, 34, _("Fat.No:") );
  mvprintw (1, 55, _("Ýrs.No:") );
  mvprintw (2, 1,  _("Kime:") );

  mvprintw (20, 2, _("Teslim alan: ") );
  mvprintw (21, 2, _("Teslim eden: ") );

 mvprintw (20, 2,  _("Teslim alan: ") );
  mvprintw (21, 2, _("Teslim eden: ") );
  mvprintw (22, 2, _("Teslim yeri: ") );

  mvprintw (20, 40, _("Fiili sevk tarihi: ") );
  mvprintw (21, 40, _("Düzenleme saati  : ") );
  mvprintw (22, 40, _("Düzenleyen       : ") );

for (i=7;i<13;i++)
{
	set_field_fore(field[i], COLOR_PAIR(2) );
	set_field_back(field[i], COLOR_PAIR(2) );
}


//kalýn yazmaya baþla
attron(A_BOLD);

//baslýk
attron (COLOR_PAIR (2));
  mvprintw (3, 0,  _(" Malzeme Kodu           ") );
  mvprintw (3, 17, _(" Ürünün Adý                     ") );
  mvprintw (3, 43, _(" Miktar  ") );
  mvprintw (3, 52, _(" Birim ") );
  mvprintw (3, 59, _(" Birim Fiyatý        ") );
  attroff (COLOR_PAIR (2));
  
attroff(A_BOLD);  
  
  //cizgi cizliecek
  for (i=4;i<19;i++) 
  {
	  mvaddch (i, 0, ACS_VLINE);
	  mvaddch (i, 17, ACS_VLINE);
	  mvaddch (i, 43, ACS_VLINE);
	  mvaddch (i, 52, ACS_VLINE);
	  mvaddch (i, 59, ACS_VLINE); 
	  mvaddch (i, 79, ACS_VLINE);
  }

//19 nolu satýrda tüm pencerenin alt kýsmý tamamlanýyor
mvaddch (19, 0, ACS_LTEE);

for (i=1;i<17;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 17, ACS_BTEE);

for (i=18;i<43;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 43, ACS_BTEE);

for (i=44;i<52;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 52, ACS_BTEE);

for (i=53;i<59;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 59, ACS_BTEE);

for (i=60;i<79;i++) mvaddch (19, i, ACS_HLINE);
mvaddch (19, 79, ACS_RTEE);

mvaddch (19, 39, ACS_TTEE);

//19 satýr iþlemleri tamamlandý

/**********************************/

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

mvprintw (24 , 0, _(" F1->Help   F2->Save    F3->New    F4->Delete  F8->Back  P/N PgUp/PgDn") );
  

/*************************************************************/
//ilk acýlýnca en son kayýt uzerine konumlan

  row_no_buyuk_tur ("irsaliyeler", "1");
  sprintf (query,
  "select row_no, tarih, fatura_no, irsaliye_no, firma_adi, talan, teden, teslim_adresi, fsevktar, duz_saat, duzenleyen from irsaliyeler where row_no=%i and tur='1' ",
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
//talan
  set_field_buffer (field[7], 0, pgsql_kayit_field() );
//teden
  set_field_buffer (field[8], 0, pgsql_kayit_field() );

//teslim_adresi
  set_field_buffer (field[9], 0, pgsql_kayit_field() );
//fiili sevk tarihi
  set_field_buffer (field[10], 0, pgsql_kayit_field() );
//duzenlme saati
  set_field_buffer (field[11], 0, pgsql_kayit_field() );
//duzenleyen
  set_field_buffer (field[12], 0, pgsql_kayit_field() );

  sprintf (query,
  "select malzeme_kodu, aciklama, giden, birim_tip, birim_fiyat from stokgiden where tarih='%s-%s-%s'  and belge_no='%s' ",
  g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
  g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[5], 0)) );

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
      i = i + 5;
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
	if ((field_index(current_field(my_form)) +5) <115) 
		set_current_field(my_form, field[field_index(current_field(my_form)) +5] );
	  break;
	case KEY_UP:
	if ((field_index(current_field(my_form)) -5) >1) 
		set_current_field(my_form, field[field_index(current_field(my_form)) -5] );
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


	bolme= div(field_index(current_field(my_form))-13, 5);

	//ürün secilmek istenirse
	if ( bolme.rem == 0) 
		{			
         form_driver (my_form, REQ_NEXT_FIELD);
         form_driver (my_form, REQ_PREV_FIELD);
         secilen = field_goster ("stokkart", "malzeme_adi", 0);
         i = 13;
        while (i < 88)
			{
			  sprintf (query, "%s",
			   rtrim (field_buffer (field[i], 0), " "));
				  if (strlen (query) < 1)
				{
				  set_field_buffer (field[i + 1], 0, secilen);
				  //field icindeki deger sql icinden okunacak ve sonaki field 
				  //icine yazýlacak urun adý
				  sprintf (query,
				   "select malzeme_kodu,malzeme_adi, birim_tip, birim_fiyat from stokkart where malzeme_adi like '%%%s%%' ",
				  g_strstrip (field_buffer (field[i + 1], 0)));
				
				  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			
				  set_field_buffer (field[i], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+1], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+3], 0, pgsql_kayit_field() );
				  set_field_buffer (field[i+4], 0, pgsql_kayit_field() );
				  				  
				  //konumlan                                      
				  form_driver (my_form, REQ_NEXT_FIELD);
				  form_driver (my_form, REQ_PREV_FIELD);
				  break;
					}
			  i = i + 5;
			}

		  form_driver (my_form, REQ_NEXT_FIELD);
		  form_driver (my_form, REQ_PREV_FIELD);

		}


		}
		break;


  	  

	case KEY_F (2):  //kaydetme iþlemleri

	  toplam = 0;

	  if (durum == 1)	//sadece yeni kayýt ise calýs
	    {
	      form_driver (my_form, REQ_NEXT_FIELD);
	      form_driver (my_form, REQ_PREV_FIELD);

	      if (strlen (rtrim (field_buffer (field[5], 0), " ")) < 1)
		{
		  mesaj ("Ýrsaliye no. yazmadýnýz.");
		  form_driver (my_form, REQ_NEXT_FIELD);
	      form_driver (my_form, REQ_PREV_FIELD);
		  break;
		}

	      if (strlen (rtrim (field_buffer (field[6], 0), " ")) < 1)
		{
		  mesaj ("Firma adý girmek zorundasýnýz.");
		  form_driver (my_form, REQ_NEXT_FIELD);
	      form_driver (my_form, REQ_PREV_FIELD);
		  break;
		}

	      if (strlen (rtrim (field_buffer (field[13], 0), " ")) < 2)
		{
		  mesaj ("Kayýt girmediniz.");
		  form_driver (my_form, REQ_NEXT_FIELD);
	      form_driver (my_form, REQ_PREV_FIELD);
		  break;
		}

	      sprintf (tarih, "%s.%s.%s", g_strstrip(field_buffer (field[3], 0)),
		  g_strstrip(field_buffer (field[2], 0)), g_strstrip(field_buffer (field[1],0))  );

	      rowno_miktar = atoi (field_buffer (field[0], 0));

	      for (i = 13; i < 88; i = i + 5)
		{
		//kontrol et aksi takdirde glib null hatasý veriyor
			if ((strlen ( rtrim(field_buffer (field[i], 0)," ")) > 2)
		      && (atoi (field_buffer (field[i + 2], 0)) > 0))
		    {			
			sprintf(query, 
			"insert into stokgiden (tarih, malzeme_kodu, belge_no, aciklama, birim_fiyat, giden, kime, tur, birim_tip) 			values ('%s','%s','%s','%s', '%s', '%s', '%s', '1', '%s')",
			/*row_no cikarildi, tarih*/
			tarih,
			/*malzeme_kodu, belge no*/
			g_strstrip(field_buffer(field[i], 0)), g_strstrip(field_buffer(field[5],0)),
			/*urun adi, birim_fiyat */
			g_strstrip(field_buffer(field[i+1],0)), g_strstrip(field_buffer(field[i+4],0)) ,
			/*mikar, kimden*/
			g_strstrip(field_buffer(field[i+2],0)) , g_strstrip(field_buffer(field[6],0)) ,
			//birim_tip
			g_strstrip(field_buffer(field[i+3],0))  );
			
			if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
			stok_miktar_isle(field_buffer(field[i], 0), atoi(field_buffer(field[i+2],0)), -1);	 
		    }
		}

		//irsaliyeler tablosuna iþlenecek
			
		row_no_buyuk("irsaliyeler");
		//kayýt iþlemleri bittikten sonra ilgili irsaliye alanýna fatura numarasý iþlenmlidir
		sprintf (query,"insert into irsaliyeler (tarih, fatura_no, irsaliye_no, firma_adi, toplam, kdv, toplam_tutar, tur, talan, teden, teslim_adresi, fsevktar, duz_saat, duzenleyen) values ('%s','%s','%s','%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')" , 
		//tarih
		tarih,
		//fatura_no, irsaliye_no
		"",  g_strstrip( field_buffer (field[5], 0) ),
		//firma_adi ,toplam
		g_strstrip(field_buffer (field[6], 0)), "", "", "",
		//tur 0 ise gelen irsaliye,  1 ise kesilen irsaliye
		"1", g_strstrip(field_buffer (field[7], 0)), g_strstrip(field_buffer (field[8], 0)),
		g_strstrip( field_buffer (field[9], 0) ), g_strstrip( field_buffer (field[10], 0) ),
		g_strstrip( field_buffer (field[11], 0) ), g_strstrip( field_buffer (field[12], 0) )
		);		
				
		if ( pgsql_sorgula(query) ==1 )	{ mesaj(sql_sorgulama_hatasi); return 1;}	


	  /*stok kayýdý yapýldýktan sonra kullanýcý girmek isteyebilir o sorulacak ve 
	     sonrasýnda field alanlarý bosaltýlacak */

	  donen_deger = onayla (yeni_kayit_istiyormusunuz);
	  if (donen_deger == 0) return;
	  if (donen_deger == 1)
	    {
	      /*evet deðeri girildi */
	      /*row_no alaný ogrenilerek 1 artýlýp ekrana yazýlacaktýr */
	      row_no_buyuk ("stokgiden");
	      set_field_buffer (field[0], 0, rowno);

	      /*tarih içine degerler yazýlýyor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

		  //duzenleyen ksýmýna kullanýcý adý yazýlacak ve aktif edilemez yapýlacak
		  set_field_buffer(field[12], 0, kullanici);
		  field_opts_off (field[12], O_ACTIVE);
			
		  sprintf(duzenleme_saati, "%s:%s:%s", saat,dakika,saniye);
		  set_field_buffer(field[11],0, duzenleme_saati);
		  field_opts_off (field[11], O_ACTIVE);

		  sprintf(sevk_tarihi, "%s.%s.%s", gun, ay, yil);
		  set_field_buffer(field[10], 0, sevk_tarihi);	


	      for (i = 4; i < 88; i++)  set_field_buffer (field[i], 0, "");
		  // irsaliye alanýna konumlan
		  set_current_field(my_form, field[5] );
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
	      row_no_buyuk ("stokgiden");
	      set_field_buffer (field[0], 0, rowno);

	      for (i = 1; i < 88; i++)
			{
		    field_opts_on (field[i], O_EDIT);
		    set_field_buffer (field[i], 0, "");
			}

	      //firma adý alaný yazýlmaz olmalý
			field_opts_off (field[6], O_EDIT);
			
		//fatura no en son olana bir ekleme yapýlarak yazýlacak

		  /*tarih içine degerler yazýlýyor */
	      /*gun ay yil degerleri yaziliyor */
	      tarih_ayarla ();

	      set_field_buffer (field[1], 0, gun);
	      set_field_buffer (field[2], 0, ay);
	      set_field_buffer (field[3], 0, yil);

		  //en buyuk irsaliye degeri okunup bir atrýlýp yazacak
		  strcpy(query, "select max(irsaliye_no) from irsaliyeler");
		  if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		  sprintf(max_fatura_no_str,"%s",pgsql_kayit_field());
		  max_fatura_no=atoi(max_fatura_no_str);
		  sprintf(max_fatura_no_str,"%i",max_fatura_no+1);
		  set_field_buffer(field[5],0,max_fatura_no_str);			
  //duzenleyen ksýmýna kullanýcý adý yazýlacak ve aktif edilemez yapýlacak
		  set_field_buffer(field[12], 0, kullanici);
		  field_opts_off (field[12], O_ACTIVE);
			
		  sprintf(duzenleme_saati, "%s:%s:%s", saat,dakika,saniye);
		  set_field_buffer(field[11],0, duzenleme_saati);
		  field_opts_off (field[11], O_ACTIVE);

		  sprintf(sevk_tarihi, "%s.%s.%s", gun, ay, yil);
		  set_field_buffer(field[10], 0, sevk_tarihi);	
		  //duzenleyen ksýmýna kullanýcý adý yazýlacak ve aktif edilemez yapýlacak
		  set_field_buffer(field[12], 0, kullanici);
		  field_opts_off (field[12], O_ACTIVE);
			
		  sprintf(duzenleme_saati, "%s:%s:%s", saat,dakika,saniye);
		  set_field_buffer(field[11],0, duzenleme_saati);
		  field_opts_off (field[11], O_ACTIVE);

		  sprintf(sevk_tarihi, "%s.%s.%s", gun, ay, yil);
		  set_field_buffer(field[10], 0, sevk_tarihi);		

	      durum = 1;	//su anda yeni kayýt konumuna gecildi
	    }

	  break;



	case KEY_F (4):	/*sil */

				//silme hakký kontrolu
				if ( haklari_kontrol_et(3)==0 )	{
				mesaj("Silme hakkýnýz yok.!!!"); 
				form_driver (my_form, REQ_NEXT_FIELD);
				form_driver (my_form, REQ_PREV_FIELD);
				break;}
				
				donen_deger=onayla(kayit_silme_onayla);
			
				if (donen_deger==1)
				{				
				sprintf(query,"delete from stokgiden where belge_no='%s' ", field_buffer(field[5],0) );
					
				if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
				
				/*************************************************************/
				
				sprintf(query,"delete from irsaliyeler where irsaliye_no='%s' ", field_buffer(field[5],0) );
					
				if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
				
				/*************************************************************/
				//silinen irsaliye stokkart içinden azaltýlacak
				
				for (i=13;i<88;i=i+5)				
				{
					if ( ( strlen( rtrim(field_buffer(field[i], 0), " ") ) >2 ) && ( atoi(field_buffer(field[i+2],0)) >0 ) )
					{					
					/*malzeme_kodu, miktar, azaltma cogaltma*/
					/*azaltma ise -1 yazilir*/
					stok_miktar_isle(field_buffer(field[i], 0),  atoi(field_buffer(field[i+2],0)) , -1);
					}
				}
								
				/*************************************************************/
				//en son kayýt uzerine konumlan
				
			  row_no_buyuk_tur ("irsaliyeler", "1");
			  sprintf (query,
			"select row_no, tarih, fatura_no, irsaliye_no, firma_adi, talan, teden from irsaliyeler where row_no=%i and tur='1' ",
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
			//talan
			  set_field_buffer (field[7], 0, pgsql_kayit_field() );
			//teden
			  set_field_buffer (field[8], 0, pgsql_kayit_field() );

			  sprintf (query,
			  "select malzeme_kodu, aciklama, giden, birim_tip, birim_fiyat, birim_fiyat from stokgiden where tarih='%s-%s-%s'  and belge_no='%s' ",
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
				  i = i + 5;
				}


				refresh();
				
				}


	  break;

/*********************************************************/

	case KEY_NPAGE:	/*geri kayýt */
		
	if (durum!=	0) break;
				
		rownno = atoi (field_buffer (field[0], 0));		
		
		//en kucuk mu diye bakmak lazým eger en kucuk ise break
		row_no_kucuk_tur("irsaliyeler","1");
		if (rownno==row_no) break;

		//rownno dan kucuk ama en buyuk kayýt bulunacak
		sprintf (query,	"select row_no, tarih, fatura_no, irsaliye_no, firma_adi, talan, teden, teslim_adresi, fsevktar, duz_saat, duzenleyen from irsaliyeler where row_no < %i and tur='1' order by row_no desc"
		,rownno);
		
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
		//row no
		set_field_buffer (field[0], 0, pgsql_kayit_field() );
		  //tarih
		  sql_tarih_ayir ( pgsql_kayit_field() );
		  set_field_buffer (field[1], 0, sql_gun);
		  set_field_buffer (field[2], 0, sql_ay);
		  set_field_buffer (field[3], 0, sql_yil);
		// fatura no
		  set_field_buffer (field[4], 0, pgsql_kayit_field() );
		  //irsaliye no
		  set_field_buffer (field[5], 0, pgsql_kayit_field() );
		  //firma adi
		  set_field_buffer (field[6], 0, pgsql_kayit_field() );
		  //talan
		  set_field_buffer (field[7], 0, pgsql_kayit_field() );
		  //teden
		  set_field_buffer (field[8], 0, pgsql_kayit_field() );
		 //teslim adresi
		  set_field_buffer (field[9], 0, pgsql_kayit_field() );
		  //ffili sevk adresi 
		  set_field_buffer (field[10], 0, pgsql_kayit_field() );
		  //duzenþme saati
		  set_field_buffer (field[11], 0, pgsql_kayit_field() );
		  //duzenleyen
		  set_field_buffer (field[12], 0, pgsql_kayit_field() );

		sprintf (query,
		"select malzeme_kodu, aciklama, giden, birim_tip, birim_fiyat from stokgiden where tarih='%s-%s-%s'  and belge_no='%s'  " ,
		g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
		g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[5], 0))
		);  
  

if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}

		  i = 13;
			for (j=0;j<pgsql_row_miktar;j++)
			{
			  set_field_buffer (field[i], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 4], 0, yazidan_paraya_cevir ( pgsql_kayit_field() ));
			  i = i + 5;
			}
			
		  //geri kalan alanlar bosaltýlýr
		  while (i < 88) { set_field_buffer (field[i], 0, "");i++;}
								
	 

	  break;


/***********************************************************************/

	case KEY_PPAGE:	/*ileri kayýt */
	if (durum!=	0) break;
	
	  //en buyukmu diye bakmak lþazim eger en buyu ise break
	  row_no_buyuk_tur("irsaliyeler","1");
	  rownno = atoi (field_buffer (field[0], 0));

      if (rownno==(row_no-1)) break;
	  		
		//rownno dan kucuk ama en buyuk kayýt bulunacak
sprintf (query,	"select row_no, tarih, fatura_no, irsaliye_no, firma_adi, talan, teden, teslim_adresi, fsevktar, duz_saat, duzenleyen from irsaliyeler where row_no > %i and tur='0' order by row_no asc"
	,rownno);
	
if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
		
			//row no
			set_field_buffer (field[0], 0, pgsql_kayit_field() );
			//tarih
			sql_tarih_ayir ( pgsql_kayit_field() );
			set_field_buffer (field[1], 0, sql_gun);
			set_field_buffer (field[2], 0, sql_ay);
			set_field_buffer (field[3], 0, sql_yil);
			//fatura no
			set_field_buffer (field[4], 0, pgsql_kayit_field() );
			//irsaliye no
			set_field_buffer (field[5], 0, pgsql_kayit_field() );
			//firma adi
			set_field_buffer (field[6], 0, pgsql_kayit_field() );
			//talan
			set_field_buffer (field[7], 0, pgsql_kayit_field() );
			//teden
			set_field_buffer (field[8], 0, pgsql_kayit_field() );
			//teslim adresi
			set_field_buffer (field[9], 0, pgsql_kayit_field() );
			//fiili sevk adresi
			set_field_buffer (field[10], 0, pgsql_kayit_field() );
			//duzenþme saati 
			set_field_buffer (field[11], 0, pgsql_kayit_field() );
			//duzenleyen
			set_field_buffer (field[12], 0, pgsql_kayit_field() );
		
		sprintf (query,
		"select malzeme_kodu, aciklama, gelen, birim_tip, birim_fiyat from stokgelen where tarih='%s-%s-%s'  and belge_no='%s'  " ,
		g_strstrip(field_buffer (field[3], 0)), g_strstrip(field_buffer (field[2], 0)),
		g_strstrip(field_buffer (field[1], 0)), g_strstrip(field_buffer (field[5], 0))
		);  
  
		if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}  
		
		  i = 13;
		for (j=0;j<pgsql_row_miktar;j++)
			{
			  set_field_buffer (field[i], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 1], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 2], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 3], 0, pgsql_kayit_field() );
			  set_field_buffer (field[i + 4], 0, yazidan_paraya_cevir ( pgsql_kayit_field() ));
			  i = i + 5;
			}
	  //geri kalan alanlar bosaltýlýr
	  while (i < 88) { set_field_buffer (field[i], 0, "");i++;}	 




		
	  break;

	
	default:
		form_driver(my_form, ch);
		break;

    }//switch sonu
	    
}//while sonu


for (i = 0; i < 88; i++) free_field (field[i]);


}//fonksiyon sonu

/*********************************************************/

