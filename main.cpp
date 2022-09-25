#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

setting instruction¨S¥´

#define GAME_TERMINATE -1
#define GAME_SUCCEEDED 777

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *image2 = NULL;
ALLEGRO_BITMAP *cell = NULL;
ALLEGRO_BITMAP *board = NULL;
ALLEGRO_BITMAP *knu1 = NULL;
ALLEGRO_BITMAP *knu2 = NULL;
ALLEGRO_BITMAP *mask = NULL;
ALLEGRO_BITMAP *tha1 = NULL;
ALLEGRO_BITMAP *tha2 = NULL;
ALLEGRO_BITMAP *gho = NULL;
ALLEGRO_BITMAP *lock = NULL;
ALLEGRO_BITMAP *gas = NULL;
ALLEGRO_BITMAP *spider = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *boardbig = NULL;
ALLEGRO_KEYBOARD_STATE keyState ;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_FONT *font = NULL;

//Custom Definition
const char *title = "Final Project 107081028";
const float FPS = 60;
const int WIDTH = 800;
const int HEIGHT = 343;
typedef struct character
{
    float x;
    float y;
    ALLEGRO_BITMAP *image_path;

}Character;

Character character1;

int get[11];
int acc[13];
int imageWidth = 0;
int imageHeight = 0;
int draw = 0;
int done = 0;
int window = 0;
int ans;
float vol = 1;
float length = 740;
bool judge_setting_window = false;
bool judge_game_window = false;
bool judge_room1_window = false;
bool judge_room2_window = false;
bool judge_room3_window = false;
bool ture = true; //true: appear, false: disappear
bool next = false; //true: trigger
bool dir = true; //true: left, false: right

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
void loading();
int escaped();
int game_run();
void game_destroy();

int main(int argc, char *argv[]) {
    int msg = 0;

    game_init();
    loading();
    game_begin();

    while (msg != GAME_TERMINATE) {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
        else if(msg == GAME_SUCCEEDED)
            msg = escaped();
    }

    game_destroy();
    return 0;
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init() {
    if (!al_init()) {
        show_err_msg(-1);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-2);
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-3);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-4);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL) {
        show_err_msg(-5);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void loading()
{
    song = al_load_sample("load.wav");
    if (!song){
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-6);
    }
    al_play_sample(song, vol, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    char s[3][20] = {"loading1.png", "loading2.png", "loading3.png"};
    for(int i = 0; i < 3; i++)
    {
        background = al_load_bitmap(s[i]);
        al_draw_bitmap(background, 0, 0, 0);
        al_flip_display();
        al_rest(1.0);
        al_destroy_bitmap(background);
    }
}

void game_begin() {
    // Loop the song until the display closes
    ///al_play_sample(song, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_clear_to_color(al_map_rgb(100,100,100));
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",20,0);
    /*al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, 550, al_map_rgb(255, 255, 255), 0);*/
    background = al_load_bitmap("back.png");
    al_draw_bitmap(background, 0, 0, 0);
    image = al_load_bitmap("title.png");
    al_draw_bitmap(image, 10, 50, 0);
    image2 = al_load_bitmap("options.png");
    al_draw_bitmap(image2, 20, 200, 0);
    al_flip_display();
}

int escaped()
{
    al_destroy_bitmap(background);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    background = al_load_bitmap("success.png");
    al_draw_bitmap(background, 0, 0, 0);
    al_flip_display();
    al_rest(5.0);
    al_destroy_bitmap(background);
    return GAME_TERMINATE;
}

int process_event(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    // Our setting for controlling animation
    if(event.timer.source == timer){
        length -= 8.9;
        if(length <= 0) return GAME_TERMINATE;
    }

    if(window == 0)
    {
        // For Start Menu
        if(event.type == ALLEGRO_EVENT_KEY_UP){
            switch(event.keyboard.keycode){
                case ALLEGRO_KEY_ENTER:
                    judge_game_window = true;
                    break;
                case ALLEGRO_KEY_S:
                    judge_setting_window = true;
                    break;
            }
        }
    }

    // Keyboard
    if(window == 1)
    {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(event.keyboard.keycode)
            {
                // Control
                case ALLEGRO_KEY_RIGHT:
                    if(character1.x + 30 <= 503.1){
                        character1.x += 30;
                        break;
                    }else if(character1.x + 30 >= 503.1 && character1.y >= 137.2 && character1.y <= 205.8){
                        judge_room2_window = true;
                        break;
                    }
                case ALLEGRO_KEY_UP:
                    if(character1.y - 30 >= 89) character1.y -= 30;
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(character1.y + 30 <= 290) character1.y += 30;
                    break;
                case ALLEGRO_KEY_LEFT:
                    if(character1.x - 30 >= 69) character1.x -= 30;
                    break;
                case ALLEGRO_KEY_X:
                    if(get[8] == 0 && character1.x >= 230.1 && character1.x <= 290.4 && character1.y >= 225.0 && character1.y <= 290)
                    {
                        get[8] = 1;
                        acc[8] = 1;
                        length -= 50;
                        break;
                    }else if(get[2] == 0 && character1.x >= 358.7 && character1.x <= 410.7 && character1.y >= 91.5 && character1.y <= 131.5)
                    {
                        get[2] = 1;
                        break;
                    }else if(character1.x >= 220.5 && character1.x <= 281.5 && character1.y >= 137.2 && character1.y <= 197.2 && get[0] == 0)
                    {
                        get[0] = 1;
                        break;
                    }else if(character1.x >= 71.5 && character1.x <= 131.5 && character1.y >= 257 && character1.y <= 330 && get[1] == 0)
                    {
                        get[1] = 1;
                        break;
                    }
            }
        }else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if(event.mouse.button == 1){
                if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 51.5 && event.mouse.y < 91.5 && get[1] == 1) acc[1] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 91.5 && event.mouse.y < 131.5 && get[2] == 1) acc[2] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 131.5 && event.mouse.y < 171.5 && get[3] == 1) acc[3] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 171.5 && event.mouse.y < 211.5 && get[4] == 1) acc[4] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 11.5 && event.mouse.y < 51.5 && get[0] == 1) acc[0] = 1;
            }
        }
    }
    if(window == 2)
    {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(event.keyboard.keycode)
            {
                // Control
                case ALLEGRO_KEY_DOWN:
                    if(character1.y + 30 <= 270){
                            character1.y += 30;
                            break;
                    }else if(character1.y + 30 >= 297 && character1.x >= 200 && character1.x <= 280){
                         judge_room1_window = true;
                        break;
                    }
                case ALLEGRO_KEY_UP:
                    if(character1.y - 30 >= 119) character1.y -= 30;
                    break;
                case ALLEGRO_KEY_LEFT:
                    if(character1.x - 30 >= 45.73) character1.x -= 30;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if(character1.x + 30 <= 457.33) character1.x += 30;
                    break;
                case ALLEGRO_KEY_X:
                    if(character1.x >= 424 && character1.x <= 484 && character1.y >= 119 && character1.y <= 150 && get[3] == 0)
                    {
                        get[3] = 1;
                        break;
                    }
                    else if(character1.x >= 89 && character1.x <= 121 && character1.y >= 119 && character1.y <= 150 && get[4] == 0)
                    {
                        get[4] = 1;
                        break;
                    }
                    else if(character1.x >= 400 && character1.x <= 484 && character1.y >= 254 && character1.y <= 297.1 && get[9] == 0)
                    {
                        get[9] = 1;
                        acc[9] = 1;
                        length -= 50;
                        break;
                    }
            }
        }else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if(event.mouse.button == 1){
                if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 51.5 && event.mouse.y < 91.5) acc[1] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 91.5 && event.mouse.y < 131.5 && get[2] == 1) acc[2] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 131.5 && event.mouse.y < 171.5 && get[3] == 1) acc[3] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 171.5 && event.mouse.y < 211.5 && get[4] == 1) acc[4] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 11.5 && event.mouse.y < 51.5 && get[0] == 1) acc[0] = 1;
                else if(event.mouse.x >= 205 && event.mouse.x <= 305 && event.mouse.y >= 0 && event.mouse.y < 150) acc[12] = 1;
            }
        }

    }
    if(window == 3)
    {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(event.keyboard.keycode)
            {
                // Control
                case ALLEGRO_KEY_UP:
                    if(character1.y - 30 >= 91.5) character1.y -= 30;
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(character1.y + 30 <= 320) character1.y += 30;
                    break;
                case ALLEGRO_KEY_LEFT:
                    if(character1.x - 30 >= 183) character1.x -= 30;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if(character1.x + 30 <= 526) character1.x += 30;
                    break;
                case ALLEGRO_KEY_X:
                    if(character1.x >= 180 && character1.x <= 220 && character1.y >= 110 && character1.y <= 160 && get[5] == 0)
                    {
                        get[5] = 1;
                        break;
                    }
                    else if(character1.x >= 329 && character1.x <= 370 && character1.y >= 110 && character1.y <= 160 && get[6] == 0)
                    {
                        get[6] = 1;
                        break;
                    }
                    else if(character1.x >= 479.1 && character1.x <= 540.1 && character1.y >= 110 && character1.y <= 160 && get[7] == 0)
                    {
                        get[7] = 1;
                        break;
                    }
                    else if(character1.x >= 449 && character1.x <= 540.1 && character1.y >= 251.5 && character1.y <= 320.13 && get[11] == 0)
                    {
                        get[11] = 1;
                        acc[11] = 1;
                        if(get[0] == 0) length -= 50;
                        break;
                    }
            }
        }else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if(event.mouse.button == 1){
                if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 251.5 && event.mouse.y < 291.5 && get[6] == 1) acc[6] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 291.5 && event.mouse.y < 331.5 && get[7] == 1) acc[7] = 1;
                else if(event.mouse.x >= 750 && event.mouse.x <= 790 && event.mouse.y >= 211.5 && event.mouse.y < 251.5 && get[5] == 1) acc[5] = 1;
            }
        }
    }



    // Shutdown our program
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;

    return 0;
}

int game_run() {
    int error = 0;
    // First window(Menu)
    if(window == 0){
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
            if(judge_game_window) {
                window = 1;
                // Setting Character
                al_destroy_bitmap(background);
                al_destroy_bitmap(image);
                al_destroy_bitmap(image2);
                al_destroy_sample(song);
                song = al_load_sample("gaming.wav");
                al_play_sample(song, vol, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                character1.x = 480;
                character1.y = 150;
                character1.image_path = al_load_bitmap("girl.png");
                background = al_load_bitmap("room1.png");
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
                al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));
                al_flip_display();
                //Initialize Timer
                timer  = al_create_timer(1.0/0.1);
                al_register_event_source(event_queue, al_get_timer_event_source(timer)) ;
                al_start_timer(timer);
            }
        }
    }
    // Second window(Main Game)
    else if(window == 1){
        // Change Image for animation
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
            if(judge_room2_window){
                window = 2;
                al_destroy_bitmap(background);
                character1.x = 240;
                character1.y = 270;
                character1.image_path = al_load_bitmap("girl.png");
                background = al_load_bitmap("room2.png");
                lock = al_load_bitmap("lock.png");
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_bitmap(lock, 205, 0, 0);
                al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
                al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));
                al_flip_display();
                judge_room2_window = false;
            }else{
            al_destroy_bitmap(background);
            character1.image_path = al_load_bitmap("girl.png");
            background = al_load_bitmap("room1.png");
            al_draw_bitmap(background, 0, 0, 0);
            al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
            al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));

            if(acc[8] == 1){
                acc[8] = 0;
                spider = al_load_bitmap("spider.png");
                al_draw_bitmap(spider, 0, 0, 0);
                al_flip_display();
                al_rest(1.5);
                al_destroy_bitmap(spider);
            }else if(acc[0] == 1){
                acc[0] = 0;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 0, 0), 200, 150, ALLEGRO_ALIGN_CENTRE, "It's a MASK");
                al_flip_display();
                al_rest(2.0);
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }else if(acc[1] == 1){
                acc[1] = 0;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "I like to play those baseball nine square!");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 150, ALLEGRO_ALIGN_LEFT, "Of course... with my sputum");
                al_draw_text(font, al_map_rgb(255, 255, 255), 500, 300, ALLEGRO_ALIGN_RIGHT, "Knuckles");
                al_flip_display();
                al_rest(4.0);
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }else if(acc[2] == 1){
                acc[2] = 0;
                knu2 = al_load_bitmap("nine.png");
                al_draw_bitmap(knu2, 0, 0, 0);
                al_flip_display();
                al_rest(1.5);
                al_destroy_bitmap(knu2);
            }else if(acc[3] == 1){
                acc[3] = 0;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 25, ALLEGRO_ALIGN_LEFT, "741258963");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, ALLEGRO_ALIGN_LEFT, "125789");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 175, ALLEGRO_ALIGN_LEFT, "74123698258");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 250, ALLEGRO_ALIGN_LEFT, "14789");
                al_flip_display();
                al_rest(4.0);
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }else if(acc[4] == 1){
                acc[4] = 0;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "So many numbers!!");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, ALLEGRO_ALIGN_LEFT, "I'd like to mod with 10^9+7");
                al_draw_text(font, al_map_rgb(255, 255, 255), 500, 300, ALLEGRO_ALIGN_RIGHT, "Thanos");
                al_flip_display();
                al_rest(4.0);
                al_clear_to_color(al_map_rgb(0, 0, 0));
            }
            if(get[2] == 1){
                knu2 = al_load_bitmap("paper.png");
                al_draw_bitmap(knu2, 750, 91.5, 0);
            }
            if(get[0] == 1){
                mask = al_load_bitmap("mask.png");
                al_draw_bitmap(mask, 750, 11.5, 0);
            }
            if(get[1] == 1){
                knu1 = al_load_bitmap("paper.png");
                al_draw_bitmap(knu1, 750, 51.5, 0);
            }
            if(get[3] == 1){
                tha1 = al_load_bitmap("paper.png");
                al_draw_bitmap(tha1, 750, 131.5, 0);
            }
            if(get[4] == 1){
                tha2 = al_load_bitmap("paper.png");
                al_draw_bitmap(tha2, 750, 171.5, 0);
            }


            al_flip_display();
        }
        }
    }else if(window == 2)
    {
        if (!al_is_event_queue_empty(event_queue)) {
        error = process_event();
        if(judge_room1_window){
            window = 1;
            al_destroy_bitmap(background);
            character1.x = 480;
            character1.y = 150;
            character1.image_path = al_load_bitmap("girl.png");
            background = al_load_bitmap("room1.png");
            al_draw_bitmap(background, 0, 0, 0);
            al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
            al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));
            al_flip_display();
            judge_room1_window = false;
        }else if(judge_room3_window){
            window = 3;
            al_destroy_bitmap(background);
            al_destroy_bitmap(lock);
            character1.x = 180;
            character1.y = 270;
            character1.image_path = al_load_bitmap("girl.png");
            background = al_load_bitmap("room3.png");
            al_draw_bitmap(background, 0, 0, 0);
            al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
            al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));
            al_flip_display();
            judge_room3_window = false;
        }else
        {
        al_destroy_bitmap(background);
        character1.image_path = al_load_bitmap("girl.png");
        background = al_load_bitmap("room2.png");
        lock = al_load_bitmap("lock.png");
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap(lock, 205, 0, 0);
        al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
        al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));

        if(acc[9] == 1){
            acc[9] = 0;
            spider = al_load_bitmap("spider.png");
            al_draw_bitmap(spider, 0, 0, 0);
            al_flip_display();
            al_rest(3.0);
            al_destroy_bitmap(spider);
        }else if(acc[0] == 1){
            acc[0] = 0;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 0, 0), 200, 150, ALLEGRO_ALIGN_CENTRE, "It's a MASK");
            al_flip_display();
            al_rest(2.0);
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }else if(acc[1] == 1){
            acc[1] = 0;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 150, ALLEGRO_ALIGN_LEFT, "I like to play those baseball nine square!");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 200, ALLEGRO_ALIGN_LEFT, "Of course... with my sputum");
            al_draw_text(font, al_map_rgb(255, 255, 255), 500, 300, ALLEGRO_ALIGN_RIGHT, "Knuckles");
            al_flip_display();
            al_rest(4.0);
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }else if(acc[2] == 1){
            acc[2] = 0;
            knu2 = al_load_bitmap("nine.png");
            al_draw_bitmap(knu2, 0, 0, 0);
            al_flip_display();
            al_rest(2.0);
            al_destroy_bitmap(knu2);
        }else if(acc[3] == 1){
            acc[3] = 0;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 25, ALLEGRO_ALIGN_LEFT, "741258963");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, ALLEGRO_ALIGN_LEFT, "125789");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 175, ALLEGRO_ALIGN_LEFT, "74123698258");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 250, ALLEGRO_ALIGN_LEFT, "14789");
            al_flip_display();
            al_rest(4.0);
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }else if(acc[4] == 1){
            acc[4] = 0;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "So many numbers!!");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, ALLEGRO_ALIGN_LEFT, "I'd like to mod with 10^9+7");
            al_draw_text(font, al_map_rgb(255, 255, 255), 500, 300, ALLEGRO_ALIGN_RIGHT, "Thanos");
            al_flip_display();
            al_rest(4.0);
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }else if(acc[12] == 1)
        {
            acc[12] = 0;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "ENTER THE PASSCODE:");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 170, ALLEGRO_ALIGN_LEFT, "you have to enter something");
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 275, ALLEGRO_ALIGN_LEFT, "if incorrect, your time decreases");
            al_flip_display();
            scanf("%d", &ans);
            if(ans == 5487) judge_room3_window = true;
            else length -= 50;
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
        if(get[2] == 1){
            knu2 = al_load_bitmap("paper.png");
            al_draw_bitmap(knu2, 750, 91.5, 0);
        }
        if(get[0] == 1){
            mask = al_load_bitmap("mask.png");
            al_draw_bitmap(mask, 750, 11.5, 0);
        }
        if(get[1] == 1){
            knu1 = al_load_bitmap("paper.png");
            al_draw_bitmap(knu1, 750, 51.5, 0);
        }
        if(get[3] == 1){
            tha1 = al_load_bitmap("paper.png");
            al_draw_bitmap(tha1, 750, 131.5, 0);
        }
        if(get[4] == 1){
            tha2 = al_load_bitmap("paper.png");
            al_draw_bitmap(tha2, 750, 171.5, 0);
        }

            al_flip_display();
        }
        }

    }
    else if(window == 3){
        if (!al_is_event_queue_empty(event_queue)) {
        error = process_event();

        al_destroy_bitmap(background);
        character1.image_path = al_load_bitmap("girl.png");
        background = al_load_bitmap("room3.png");
        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
        al_draw_filled_rectangle(5, 330, length, 335, al_map_rgb(255, 0, 0));

        if(acc[11] == 1){
            acc[11] = 0;
            gas = al_load_bitmap("gas.png");
            al_draw_bitmap(gas, 0, 0, 0);
            al_flip_display();
            al_rest(3.0);
            al_destroy_bitmap(gas);
        }else if(acc[6] == 1){
                acc[6] = 0;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 150, ALLEGRO_ALIGN_LEFT, "Key in who I am! Knuc8? T6ano? Ghoul?");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 200, ALLEGRO_ALIGN_LEFT, "Be5in a5ort! It's a toug6 questio6!");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 250, ALLEGRO_ALIGN_LEFT, "S9ide up. Goo3 lu3k. AA1AA1A!");
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 300, ALLEGRO_ALIGN_LEFT, "T6e keybo1rd is a hi6t, Ice bir3!");
                al_flip_display();
                al_rest(10.0);
                al_clear_to_color(al_map_rgb(0, 0, 0));
        }else if(acc[5] == 1){
                acc[5] = 0;
                boardbig = al_load_bitmap("boardbig.png");
                al_draw_bitmap(boardbig, 0, 0, 0);
                al_flip_display();
                al_rest(3.0);
                al_destroy_bitmap(boardbig);
        }else if(acc[7] == 1){
                acc[7] = 0;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 150, ALLEGRO_ALIGN_LEFT, "enter the name(first five letters):");
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 250, ALLEGRO_ALIGN_LEFT, "don't enter with a normal way");
                al_flip_display();
                scanf("%d", &ans) ;
                if(ans == 56979){
                    return GAME_SUCCEEDED;
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                }else length -= 50;
                al_rest(4.0);
                al_clear_to_color(al_map_rgb(0, 0, 0));
        }
        if(get[0] == 1){
            mask = al_load_bitmap("mask.png");
            al_draw_bitmap(mask, 750, 11.5, 0);
        }
        if(get[5] == 1){
            board = al_load_bitmap("boardsmall.png");
            al_draw_bitmap(board, 750, 211.5, 0);
        }
        if(get[6] == 1){
            gho = al_load_bitmap("paper.png");
            al_draw_bitmap(gho, 750, 251.5, 0);
        }
        if(get[7] == 1){
            cell = al_load_bitmap("phone.png");
            al_draw_bitmap(cell, 750, 291.5, 0);
        }
            al_flip_display();
        }
    }
    return error;
}

void game_destroy() {
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_bitmap(image);
    al_destroy_bitmap(image2);
    al_destroy_bitmap(board);
    al_destroy_bitmap(cell);
    al_destroy_bitmap(knu1);
    al_destroy_bitmap(knu2);
    al_destroy_bitmap(mask);
    al_destroy_bitmap(tha1);
    al_destroy_bitmap(tha2);
    al_destroy_bitmap(gho);
    al_destroy_bitmap(lock);
    al_destroy_bitmap(gas);
    al_destroy_bitmap(spider);
    al_destroy_bitmap(boardbig);
    al_destroy_font(font);
    al_destroy_bitmap(background);
    al_destroy_sample(song);
}
