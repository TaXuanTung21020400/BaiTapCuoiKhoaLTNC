
#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "PlayerPower.h"
#include "TextObject.h"

//Tạo các font chữ 

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_menu = NULL;
TTF_Font* end_game_text = NULL;

bool Init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) == GA_FAILED)
  {
    return false;
  }

  g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

  if (g_screen == NULL)
    return false;

  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == GA_FAILED)
    return false;

  //Chạy các file nhạc cho game
  g_sound_bullet[0] = Mix_LoadWAV(g_name_audio_bullet_main);
  g_sound_bullet[1] = Mix_LoadWAV(g_name_audio_bullet_main2);
  g_sound_exp[0] = Mix_LoadWAV(g_name_audio_exp_threats);
  g_sound_exp[1] = Mix_LoadWAV(g_name_audio_exp_main);
  g_sound_start[0] = Mix_LoadWAV(g_name_start);


  if (g_sound_exp[0] == NULL || g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_exp[1] == NULL || g_sound_start[0] == NULL )
  {
    return false;
  }

  if (TTF_Init() == GA_FAILED)
  {
    return false;
  }

  //Font chữ và cỡ chữ trong game
  g_font_text = TTF_OpenFont("Xerox Sans Serif Wide Bold.ttf", 20);
  g_font_menu = TTF_OpenFont("Xerox Sans Serif Wide Bold.ttf", 50);
  end_game_text = TTF_OpenFont("Xerox Sans Serif Wide Bold.ttf", 50);

  if (g_font_text == NULL || g_font_menu == NULL || end_game_text == NULL )
  {
    return false;
  }

  return true;
}


int main(int arc, char*argv[])
{
  bool is_run_screen = true;
  int bkgn_x = 0;
  bool is_quit = false;
  if (Init() == false)
    return 0;

  g_bkground = SDLCommonFunc::LoadImage(g_name_bk_ground);
  if (g_bkground == NULL)
  {
    return 0;
  }
  
  //Tạo mạng cho nhân vật
  PlayerPower player_power;
  player_power.Init();

  //Thời gian cho game
  TextObject time_game;
  time_game.SetColor(TextObject::WHITE_TEXT);

  //Điểm số cho game
  TextObject mark_game;
  mark_game.SetColor(TextObject::RED_TEXT);

  //Chữ xuất hiện khi dành chiến thắng
  TextObject win_game;
  win_game.SetColor(TextObject::RED_TEXT);

  //Khởi tạo máy bay điều khiển
  MainObject plane_object;
  plane_object.SetRect(POS_X_START_MAIN_OBJ, POS_Y_START_MAIN_OBJ);
  bool ret = plane_object.LoadImg(g_name_main_obj);
  if (!ret)
  {
    return 0;
  }

  //Vụ nổ xảy ra khi va chạm
  ExplosionObject exp_main;
  ExplosionObject exp_threats;
  ret = exp_main.LoadImg(g_name_exp_main);
  exp_main.set_clip();
  if (ret == false) return 0;

  ret = exp_threats.LoadImg(g_name_exp_threats );
  exp_threats.set_clip();
  if (!ret) return 0;

  //Khởi tạo máy bay địch
  ThreatObject* p_threats = new ThreatObject[NUM_THREATS];

  for (int t = 0; t < NUM_THREATS; t++)
  {
    ThreatObject* p_threat = (p_threats + t);
    if (p_threat)
    {
      ret = p_threat->LoadImg(g_name_threats);
      if (ret == false)
      {
        delete [] p_threats;
        SDLCommonFunc::CleanUp();
        SDL_Quit();
        return 0;
      }

      int rand_y = SDLCommonFunc::MakeRandValue();
      p_threat->SetRect(SCREEN_WIDTH + t*VAL_OFFSET_START_POST_THREATS, rand_y);
      p_threat->set_x_val(SPEED_THREAT);

      BulletObject* p_bullet = new BulletObject();
      p_threat->InitBullet(p_bullet);
    }
  }


  //Khởi tạo boss cuối
    ThreatObject* p_boss = new ThreatObject();

    ret = p_boss->LoadImg(g_name_bossthreat);
    if (ret == false)
    {
        return 0;
    }

    p_boss->SetRect(1100 , 500);
	p_boss->set_y_val(SPEED_THREAT * 0.6 );

    BulletObject* p_bullet = new BulletObject();
    p_boss->InitBullet(p_bullet);

  unsigned int die_number = 0;
  unsigned int mark_value = 0;
  unsigned int num_fire_boss = 0;


  // Di chuyển màn hình
  int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_menu);
  if (ret_menu == 1)
    is_quit = true;  
    Mix_PlayChannel(-1, g_sound_start[0], 0);
	
  while (!is_quit) 
  {
    while (SDL_PollEvent(&g_even)) 
    {
      if (g_even.type == SDL_QUIT)
      {
        is_quit = true;
        break;
      }
      plane_object.HandleInputAction(g_even, g_sound_bullet);
    }
    if (is_run_screen == true)
    {
      bkgn_x -= SPEED_SCREEN;
      if (bkgn_x <= -(WIDTH_BACKGROUND - SCREEN_WIDTH))
      {
        is_run_screen = false;
      }
      else
      {
        SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x, 0);
      }
    }
    else
    {
      SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x, 0);

	  //boss xuất hiện khi chạy mết màn hình

	  p_boss->HandleMoveBoss(SCREEN_WIDTH, SCREEN_HEIGHT);
      p_boss->Show(g_screen);
      p_boss->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);


	  //va chạm giữa đạn của boss với máy bay mình điều khiển
	  bool is_col2 = false;
        std::vector<BulletObject*> bullet_arr = p_boss->GetBulletList();
        for (int am = 0; am < bullet_arr.size(); am++)
        {
          BulletObject* p_bullet = bullet_arr.at(am);
          if (p_bullet)
          {
            is_col2 = SDLCommonFunc::CheckCollision(p_bullet->GetRect(), plane_object.GetRect());
            if (is_col2 == true)
            {
              p_boss->ResetBullet(p_bullet);
              break;
            }
          }
        }

		bool is_col4 = SDLCommonFunc::CheckCollision(plane_object.GetRect(), p_boss->GetRect());
		if (is_col4 || is_col2  )
        {
          for (int ex = 0; ex < 4; ex++)
          {
            int x_pos = (plane_object.GetRect().x + plane_object.GetRect().w*0.5) -  EXP_WIDTH*0.5;
            int y_Pos = (plane_object.GetRect().y + plane_object.GetRect().h*0.5)- EXP_HEIGHT*0.5;

            exp_main.set_frame(ex);
            exp_main.SetRect(x_pos, y_Pos);
            exp_main.ShowEx(g_screen);

            //Update screen
            if ( SDL_Flip(g_screen) == -1)
            {
              delete [] p_threats;
              SDLCommonFunc::CleanUp();
              SDL_Quit();
              return 0;
            }
          }

           Mix_PlayChannel(-1, g_sound_exp[1], 0); 

           die_number++;
           if (die_number <= 2)
           {
             SDL_Delay(1000);
             plane_object.SetRect(POS_X_START_MAIN_OBJ, POS_Y_START_MAIN_OBJ);
             player_power.Decrease();
             player_power.Render(g_screen);

             if (SDL_Flip(g_screen) == -1)
             {
               delete [] p_threats;
               SDLCommonFunc::CleanUp();
               SDL_Quit();
               return 0;
             }
           }
           else
           {
             if (MessageBox(NULL, L"GAME OVER!", L"Info", MB_OK) == IDOK)
             {
               delete [] p_threats;
               SDLCommonFunc::CleanUp();
               SDL_Quit();
               return 1;
             }
           }
        }

		//

            std::vector<BulletObject*> amo_list = plane_object.GetBulletList();
            for (unsigned int am = 0; am < amo_list.size(); am++)
            {
                BulletObject* p_bullet = amo_list.at(am);
                if (p_bullet != NULL)
                {
                    bool ret_collis = SDLCommonFunc::CheckCollision(p_bullet->GetRect(), p_boss->GetRect());
                    if (ret_collis)
                    {
                        num_fire_boss++;
                        for (int ex = 0; ex < 2; ex++)
                        {
                            int x_pos = p_boss->GetRect().x  - EXP_WIDTH*0.5;
                            int y_pos = p_boss->GetRect().y  - EXP_HEIGHT*0.5;

                            exp_main.set_frame(ex);
                            exp_main.ShowEx(g_screen);
							exp_main.SetRect(x_pos, y_pos);

                            // Update screen
                            if ( SDL_Flip(g_screen) == -1)
                                return 0;
                        }
                        plane_object.RemoveBullet(am);
                        if (num_fire_boss > 20)
                        {
                            std::string text("Congratulation !!! You win.");
                            win_game.SetText(text);
                            win_game.SetRect(200, 300);
                            win_game.CreateGameText(end_game_text, g_screen);
                            Mix_PlayChannel(-1, g_sound_exp[0], 0);
                            for (int ex = 0; ex < 4; ex++)
                            {
                                int x_pos = p_boss->GetRect().x   - EXP_WIDTH*0.5;
                                int y_pos = p_boss->GetRect().y   - EXP_HEIGHT*0.5;

                                exp_main.set_frame(ex);
                                exp_main.SetRect(x_pos, y_pos);
                                exp_main.ShowEx(g_screen);
                                SDL_Delay(400);
                                // Update screen
                                if ( SDL_Flip(g_screen) == -1)
                                    return 0;
                            }
							    SDL_Delay(400);
                                delete[] p_threats; // Giải phóng bộ nhớ
                                SDLCommonFunc::CleanUp();
                                SDL_Quit();
                                return 0;
                        }
                    }
                }
            }

    }
    
    //Xuất hiện số mạng của máy bay
    player_power.Render(g_screen);

    plane_object.HandleMove();
    plane_object.Show(g_screen);
    plane_object.MakeBullet(g_screen);

    //Implement Threats Object
    for (int tt = 0; tt < NUM_THREATS; tt++)
    {
      ThreatObject* p_threat = (p_threats + tt);
      if (p_threat)
      {
        p_threat->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
        p_threat->Show(g_screen);
        p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);


        //va chạm giữa đạn máy bay địch với máy bay điều khiển
        bool is_col1 = false;
        std::vector<BulletObject*> bullet_arr = p_threat->GetBulletList();
        for (int am = 0; am < bullet_arr.size(); am++)
        {
          BulletObject* p_bullet = bullet_arr.at(am);
          if (p_bullet)
          {
            is_col1 = SDLCommonFunc::CheckCollision(p_bullet->GetRect(), plane_object.GetRect());
            if (is_col1 == true)
            {
              p_threat->ResetBullet(p_bullet);
              break;
            }
          }
        }



        //Va chạm giữa máy bay điều khiển và máy bay địch
        bool is_col3 = SDLCommonFunc::CheckCollision(plane_object.GetRect(), p_threat->GetRect());
		if (is_col1  || is_col3  )
        {
          for (int ex = 0; ex < 4; ex++)
          {
            int x_pos = (plane_object.GetRect().x + plane_object.GetRect().w*0.5) -  EXP_WIDTH*0.5;
            int y_Pos = (plane_object.GetRect().y + plane_object.GetRect().h*0.5)- EXP_HEIGHT*0.5;

            exp_main.set_frame(ex);
            exp_main.SetRect(x_pos, y_Pos);
            exp_main.ShowEx(g_screen);
            //Update screen
            if ( SDL_Flip(g_screen) == -1)
            {
              delete [] p_threats;
              SDLCommonFunc::CleanUp();
              SDL_Quit();
              return 0;
            }
          }

           Mix_PlayChannel(-1, g_sound_exp[1], 0); 

           die_number++;
           if (die_number <= 2)
           {
             SDL_Delay(1000);
             plane_object.SetRect(POS_X_START_MAIN_OBJ, POS_Y_START_MAIN_OBJ);
             player_power.Decrease();
             player_power.Render(g_screen);

             if (SDL_Flip(g_screen) == -1)
             {
               delete [] p_threats;
               SDLCommonFunc::CleanUp();
               SDL_Quit();
               return 0;
             }
           }
           else
           {
             if (MessageBox(NULL, L"GAME OVER!", L"Info", MB_OK) == IDOK)
             {
               delete [] p_threats;
               SDLCommonFunc::CleanUp();
               SDL_Quit();
               return 1;
             }
           }
        }



        //Va chạm giữa đạn máy bay điều khiển và máy bay địch
        std::vector<BulletObject*> bullet_list = plane_object.GetBulletList();
        for (int im = 0; im < bullet_list.size(); im++)
        {
          BulletObject* p_bullet = bullet_list.at(im);
          if (p_bullet != NULL)
          {
            bool ret_col = SDLCommonFunc::CheckCollision(p_bullet->GetRect(), p_threat->GetRect());
            if (ret_col)
            {

              mark_value++;
              for (int tx = 0; tx < 4; tx++)
              {
                int x_pos = p_bullet->GetRect().x - EXP_WIDTH*0.5;
                int y_pos = p_bullet->GetRect().y - EXP_HEIGHT*0.5;

                exp_threats.set_frame(tx);
                exp_threats.SetRect(x_pos, y_pos);
                exp_threats.ShowEx(g_screen);

                if (SDL_Flip(g_screen) == -1)
                {
                  delete [] p_threats;
                  SDLCommonFunc::CleanUp();
                  SDL_Quit();
                  return 0;
                }
              }

              p_threat->Reset(SCREEN_WIDTH + tt*VAL_OFFSET_START_POST_THREATS);
              plane_object.RemoveBullet(im);
              Mix_PlayChannel(-1, g_sound_exp[0], 0);
            }
          }
        }
      }
    }

    //Xuất hiện thời gain cho game
    std::string str_time = "Time : ";
    Uint32 time_val = SDL_GetTicks()/1000;
    std::string str_val = std::to_string(time_val);
    str_time += str_val;

    time_game.SetText(str_time);
    time_game.SetRect(SCREEN_WIDTH - 200, 10);
    time_game.CreateGameText(g_font_text, g_screen);

    //Xuất hiện số lượng máy bay tiêu diệt
    std::string val_str_mark = std::to_string(mark_value);
    std::string strMark("Mark: ");
    strMark += val_str_mark;

    mark_game.SetText(strMark);
    mark_game.CreateGameText(g_font_text, g_screen);

    //Update screen
    if ( SDL_Flip(g_screen) == -1)
    {
      delete [] p_threats;
      SDLCommonFunc::CleanUp();
      SDL_Quit();
      return 0;
    }
  }

  delete [] p_threats;
  SDLCommonFunc::CleanUp();
  SDL_Quit();

  return 1;
}