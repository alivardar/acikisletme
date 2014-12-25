
WINDOW *my_menu_winn;
	
int yardim()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;	
	int n_choices, i;
	ITEM *cur_item;	

	
	char *choices[] = {
	    _("Stock Module"),
	    _("Current Module Using"),
	    _("Configuration Using"),
	    _("Reports Using"),
	    _("Application Licence"),
	    _("Bug Report"),
	    _("Back"),
            (char *)NULL,
                  };
	
	(int) signal (SIGINT, sonlandir);
	(int) signal (SIGILL, sonlandir);
	(int) signal (SIGTERM, sonlandir);	
	
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/*ana pencere hepsini kapsamalý*/
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

	my_menu_winn = newwin(15, 50, 5, 10);
	keypad(my_menu_winn, TRUE);

	set_menu_win(my_menu, my_menu_winn);
	set_menu_sub(my_menu, derwin(my_menu_winn, 10, 40, 4, 2));

	set_menu_mark(my_menu, mark);

	box(my_menu_winn, 0, 0);
	print_in_middle(my_menu_winn, 1, 0, 45, _("Help List"), COLOR_PAIR(1));
	mvwaddch(my_menu_winn, 2, 0, ACS_LTEE);
	mvwhline(my_menu_winn, 2, 1, ACS_HLINE, 48);
	mvwaddch(my_menu_winn, 2, 49, ACS_RTEE);
        
	post_menu(my_menu);
	wrefresh(my_menu_winn);
	touchwin(my_menu_winn);
	
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
						//Stok Sistemi Kullanýmý
						system("mcedit -d -v ../dokumanlar/stok.txt");
						
						baslik_goruntule();
						touchwin(my_menu_winn);
			    	    wrefresh(my_menu_winn);

						break;
					
					case 2:	
						
						//Cari Sistemi Kullanýmý
						system("mcedit -d -v ../dokumanlar/cari.txt");
						
						baslik_goruntule();
						touchwin(my_menu_winn);
			    	    wrefresh(my_menu_winn);
	
						break;
					
					case 3:	
						
						//Ayarlar Kullanýmý
						system("mcedit -d -v ../dokumanlar/ayarlar.txt");
						
						baslik_goruntule();
						touchwin(my_menu_winn);
			    	    wrefresh(my_menu_winn);
	
						break;					
					
    				case 4:	
	
						//Raporlar Kullanýmý
						system("mcedit -d -v ../dokumanlar/raporlar.txt");
						
						baslik_goruntule();
						touchwin(my_menu_winn);
			    	    wrefresh(my_menu_winn);
	
				    break;
					
					case 5: 
						
						//Lisans Anlaþmasý
						system("mcedit -d -v ../dokumanlar/lisans.txt");
					
						baslik_goruntule();
						touchwin(my_menu_winn);
			    	    wrefresh(my_menu_winn);
						
						break;
					
					case 6: 
						//hata bildirim tanýtým ekraný
						hata_bildirimi();
						baslik_goruntule();
						touchwin(my_menu_winn);
			    	    wrefresh(my_menu_winn);
					
						break;
					
					case 7: //Çýkýþ
						return 0;					
						break;
				  
						}//switch sonu
						   break;
						
				refresh();
				pos_menu_cursor(my_menu);
		}
                wrefresh(my_menu_winn);
	}			
        
		return 0;
}




int hata_bildirimi()
{
    WINDOW *abwin;
    int c;
    abwin = newwin(15, 70, 5, 5);
    box(abwin, 0, 0);

/*banner yazýlacak*/
    mvwprintw(abwin, 2,1," %-55s ","      |    .---  --- | )    o     |         |              ");
    mvwprintw(abwin, 3,1," %-55s ","     | |   |      |  |-     .,---.|    ,---.|--- ,-.-.,---.");
    mvwprintw(abwin, 4,1," %-55s ","    |---|  |      |  | )    |`---.|    |---'|    | | ||---'");
    mvwprintw(abwin, 5,1," %-55s ","   |     | .---  --- |  )   ``---'`---'`---'`---'` ' '`---'");

    mvwprintw(abwin, 7,1," %-55s ", "          Acik isletme ");
    mvwprintw(abwin, 8,1," %-55s ", _("   All Rights Reserved (c) 2001-2005  M.Ali VARDAR") );
    mvwprintw(abwin, 9,1," %-55s ", _("      URL   : http://www.linuxprogramlama.com") );
    mvwprintw(abwin, 10,1," %-55s ", _("     Email : programlama@linuxprogramlama.com") );
    mvwprintw(abwin, 11,1," %-55s ", _("if you like that this project please donate us.") );
    
    wmove(abwin, 7,57);

    touchwin(abwin);
    wrefresh(abwin);

    while (1) {
	switch(wgetch(ana_win))
	{
	case 10:
	    delwin(abwin);
	    touchwin(ana_win);
	    wrefresh(ana_win);
	    touchwin(my_menu_winn);
	    wrefresh(my_menu_winn);
	    return 1;
	}
    }
    return 0;
}


