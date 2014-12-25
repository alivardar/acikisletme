/***************************************************************************
 *            main.c
 *
 *  Thu Mar 10 22:34:22 2005 - acikisletme release 2.1
 *  Copyright  2001-2005  M.Ali VARDAR
 *  ali@linuxprogramlama.com
 ****************************************************************************/


//base configuration files
#include "../include/conf.h"

//market module
#include "../include/market.h"

//currency module
#include "../include/cari.h"
#include "../include/faturakesilen.h"
#include "../include/faturagelen.h"

//stock module
#include "../include/stok.h"
#include "../include/irsaliyegiris.h"
#include "../include/irsaliyecikis.h"

//cheque module
#include "../include/ceksenet.h"
#include "../include/ayarlar.h"
#include "../include/yardim.h"
#include "../include/raporlar.h"



int main()
{
setlocale(LC_MESSAGES,"");
setlocale(LC_CTYPE,"");
bindtextdomain("acikisletme","/usr/local/share/locale");
textdomain("acikisletme");

main_giris();
}




int hakkinda() 
{
    WINDOW *abwin;
    	
    abwin = newwin(15, 70, 5, 5);
    box(abwin, 0, 0);

/*banner*/
    mvwprintw(abwin, 7,1," %-55s ", _("   acik isletme  ") );
    mvwprintw(abwin, 8,1," %-55s ", _("  GPL Release (c) 2001-2005  M.Ali VARDAR") );
    mvwprintw(abwin, 9,1," %-55s ", _("      URL:  http://www.linuxprogramlama.com") );
    mvwprintw(abwin, 10,1," %-55s ", _("        Email: ali@linuxprogramlama.com") );
    mvwprintw(abwin, 11,1," %-55s ", "");
    
    wmove(abwin, 7,57);

    touchwin(abwin);
    wrefresh(abwin);

    while (1) {
	switch(wgetch(abwin))
	{
	case 10:
	    delwin(abwin);
	    touchwin(ana_win);
	    wrefresh(ana_win);
	    return 0;
	case 27:	
	    delwin(abwin);
	    touchwin(ana_win);
	    wrefresh(ana_win);
	    return 0;
	}
    }
    return 0;
}





int main_giris()
{	
	ITEM **my_items;
	int c;				
	MENU *my_menu;
	int n_choices, i;
	ITEM *cur_item;	
	WINDOW *my_menu_win;
	
char *anamenu[] = {
        _("Market ") ,
        _("Current Module"),
	_("Cheque Module"),
	_("Stock Module"),
        _("Reports"),
        _("Configuration"),
        _("Help"),
        _("About"),
        _("Exit"),
        (char *)NULL,
                  };
	  
  	(int) signal (SIGINT, sonlandir);
  	(int) signal (SIGILL, sonlandir);
  	(int) signal (SIGTERM, sonlandir);	
		  
	//backup control
	yedek_kontrol_et();

	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	//sql configuration 
	ayar_dosyasi_oku ();
	
	ana_win = newwin(LINES, 80, 0, 0);
	temizle_win=newwin(LINES, 80, 0, 0);
	
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	
	terminal_kontrol_et();
	if (irsaliye_gun_kontrol())  mesaj( _("Some bills not prepared. Don't forget.!") );
			
	kullanici_onayla();
	
	init_pair(1, COLOR_WHITE, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	
	baslik_goruntule();

    	n_choices = ARRAY_SIZE(anamenu);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
	my_items[i] = new_item(anamenu[i], " ");

	my_menu = new_menu((ITEM **)my_items);

	my_menu_win = newwin(15, 50, 5, 10);
	keypad(my_menu_win, TRUE);
     
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 10, 40, 4, 2));

	set_menu_mark(my_menu, mark);

	box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 45, "Options", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 48);
	mvwaddch(my_menu_win, 2, 49, ACS_RTEE);
        
	post_menu(my_menu);
	wrefresh(my_menu_win);

	while((c = wgetch(my_menu_win)) )
	{       switch(c)
	        {				
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
				case 1:	/* Kasa satis */
						//satis
						if ( haklari_kontrol_et(0)==1 )	market_ana_ekran();
							else {mesaj(_("Access denied.!!!"));}
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();
						refresh();
						break;
					
				case 2:	/*cari*/
						//stok giris kontrolu
						if ( haklari_kontrol_et(0)==1 )	cari();
							else {mesaj(_("Access denied.!!!"));}
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();
						refresh();
						break;
	    			case 3:	/*ceksenet*/
						
						//cari giris kontrolu
						if ( haklari_kontrol_et(1)==1 )	ceksenet();
							else {mesaj(_("Access denied.!!!"));}
						baslik_goruntule();						
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();						
						refresh();
						break;
							
				case 4:	/*stok*/
						
						//cari giris kontrolu
						if ( haklari_kontrol_et(1)==1 )	stok();
							else {mesaj(_("Access denied.!!!"));}
						baslik_goruntule();						
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();						
						refresh();
						break;
							
							
				case 5:	/*raporlar*/

						//rapor giris kontrolu
						if ( haklari_kontrol_et(2)==1 ) raporlar();
						else
						{mesaj(_("Access denied.!!!"));}
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();
						refresh();
						break;
						
				case 6:	/*ayarlar*/
						
						ayarlar();
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();						
						refresh();
						break;
					
				case 7:/*yardým*/
						
						yardim();

						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();
						refresh();
						break;
					
				case 8:/*hakkýnda*/
						hakkinda();
					
						baslik_goruntule();
						touchwin(my_menu_win);
						wrefresh(my_menu_win);
						baslik_yaz();
						refresh();
						break;
					
				case 9:/*Kapat*/
					donen_deger=onayla(cikis_onay);
						if (donen_deger==1)
								{
								// donen deger evet ise kapat
								beep();
								touchwin(ana_win);
								wrefresh(ana_win);
								unpost_menu(my_menu);
								free_menu(my_menu);
								for(i = 0; i < n_choices; ++i)
								free_item(my_items[i]);
								endwin();
								return;
								}
						touchwin(ana_win);
						wrefresh(ana_win);								
						touchwin(my_menu_win);
						wrefresh(my_menu_win);		
						break;	
	        }

				wrefresh(my_menu_win);
				pos_menu_cursor(my_menu);
		}
                wrefresh(my_menu_win);
	}	

}






int kullanici_onayla()
{
	FIELD *field[3];
	FORM  *my_form;
	int ch;

	touchwin(ana_win);
	wrefresh(ana_win);
			
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
		
/*calisma yili 4 karakter*/
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

/*sifre alani olan ikinci field password bi�iminde olmali/
	field_opts_off(field[1], O_PUBLIC);

	/*geri plan rengi olmasin*/
/*	set_field_back(field[0], A_UNDERLINE); 
	set_field_back(field[1], A_UNDERLINE); 
*/
	my_form = new_form(field);
	post_form(my_form);
	refresh();
	
	set_current_field(my_form, field[0]); 

	mvprintw(18, 25, _("User name :") );
	mvprintw(20, 25, _("Password  :") );

	attron (COLOR_PAIR (2));
	mvprintw (0, 0, baslik);
	mvprintw (LINES-1 , 0, baslik);
	attroff (COLOR_PAIR (2));

attron(A_BLINK);
attron(A_BOLD);
mvprintw(2, 25, _("This application under GPL license.") );
attroff(A_BLINK);
attroff(A_BOLD);

attron(A_BOLD);
mvprintw(LINES-3, 5, _("F2-> Begin                                               ESC-> Cancel") );
mvprintw(18, 5, _("F5-> Settings") );
attroff(A_BOLD);

mvprintw(7,3, _("                               Acik                          "));
mvprintw(8,3,"  _|              _|                _|                                  ");
mvprintw(9,3,"         _|_|_|   _|     _|_|     _|_|_|_|   _|_|_|  _|_|       _|_|    ");
mvprintw(10,3,"  _|   _|_|       _|   _|_|_|_|     _|       _|    _|    _|   _|_|_|_|  ");
mvprintw(11,3,"  _|       _|_|   _|   _|           _|       _|    _|    _|   _|        ");
mvprintw(12,3,"  _|   _|_|_|     _|     _|_|_|       _|_|   _|    _|    _|     _|_|_|  ");
refresh();
    while((ch = getch()) )
	{	switch(ch)
		{	
		case 8: /*KEY_BACKSPACE hatali kod vermekte 8 dogru olandir konsolda 8 xterm key_backspace*/
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
			break;
		case KEY_BACKSPACE: 
    		form_driver(my_form, REQ_PREV_CHAR);
			form_driver(my_form, REQ_DEL_CHAR);
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
		case 27: /*esc durumunda*/
				unpost_form(my_form);
				free_form(my_form);
				free_field(field[0]);
				free_field(field[1]); 
				endwin();
				exit(0);	
				break;
		
		case KEY_F(5): 	/*ayarlara gecis*/
				ayarlar();
				
				unpost_form(my_form);
				free_form(my_form);
				free_field(field[0]);
				free_field(field[1]);
				
				endwin();
		
				exit (0);
				break;
				case KEY_F(2): 	/*f2 durumunda*/
				form_driver(my_form, REQ_PREV_FIELD);
				strcpy(query, "select kullanici, sifre, haklar from kullanicilar");
		
				if ( pgsql_sorgula(query) ==1 ) { mesaj(sql_sorgulama_hatasi); return 1;}
	
				for (i=0;i<pgsql_row_miktar;i++)
					{						
					sprintf(kullanici,"%s", pgsql_kayit_field() );
					sprintf(sifre,"%s", pgsql_kayit_field() );
					sprintf(haklar,"%s", pgsql_kayit_field() );

					if ( strcmp(g_strstrip(field_buffer(field[0],0)), kullanici) ==NULL )
						{
						if  ( (sifre_kripto_coz( g_strstrip(field_buffer(field[1],0)) , sifre) == 0) || (strcmp(g_strstrip(field_buffer(field[1],0)), sifre) == NULL ) )
							{
							//kullanýcý root ise haklar full olmalý 
							//databaseden degistirme ihtimali olabilir								
							if ( strstr( kullanici, "root") ) 
							{sprintf(haklar,"%s", "1111111111111111111111111111111111111111111111111111111111111111");}
												
							beep();
						
							unpost_form(my_form);
							free_form(my_form);
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
							//beep sesi							
							beep();
							
							}
						}
					}
			
													
				break;
			
			default:
				form_driver(my_form, ch);
				break;

		}
	}

return 0;
/*son*/
}
