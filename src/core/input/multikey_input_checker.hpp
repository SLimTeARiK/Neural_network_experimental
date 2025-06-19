#pragma one



class multikey_input_checker{

std::vector<int> pressed_keys;
ALLEGRO_KEYBOARD_STATE key_state;

public:
int pressed{0};
multikey_input_checker(){}
~multikey_input_checker(){}

inline void update() {
    al_get_keyboard_state(&key_state);

    pressed_keys.clear(); // Очищаем массив перед сохранением новых данных
    pressed = 0;

    for (int i = 0; i < ALLEGRO_KEY_MAX; ++i) {
        if (al_key_down(&key_state, i)) {
            pressed_keys.push_back(i); // Добавляем код клавиши в массив
            pressed++;
        }
    }
}




// Функция для проверки состояния клавиш
bool check_keys_pressed(const std::vector<int>& keys_to_check)const{
        //if (pressed != keys_to_check.size()) {
            //return false;
        //}


for(int i{0};i < pressed;i++){
    if(pressed_keys[i] == keys_to_check[0] || pressed_keys[i] == keys_to_check[1]){
        //break;
    }
    else{
        return false; // Если найдена клавиша, не входящая в список, возвращаем false
    }
}

return true; // Все клавиши совпадают
}
};
/*
template<int... Args>
void compareAndProcess(ALLEGRO_EVENT& event) {
    // Используем оператор фолдинга для обработки всех аргументов
    if((event.keyboard.keycode == Args || ...)){

    }
}


    compareAndProcess(event, )
switch (event.type) {
ALLEGRO_KEY_A
case :
    break;
ALLEGRO_KEY_B
case :
    break;
ALLEGRO_KEY_C
case :
    break;
ALLEGRO_KEY_D
case :
    break;
ALLEGRO_KEY_E
case :
    break;
ALLEGRO_KEY_F
case :
    break;
ALLEGRO_KEY_G
case :
    break;
ALLEGRO_KEY_H
case :
    break;
ALLEGRO_KEY_I
case :
    break;
ALLEGRO_KEY_J
case :
    break;
ALLEGRO_KEY_K
case :
    break;
ALLEGRO_KEY_L
case :
    break;
ALLEGRO_KEY_M
case :
    break;
ALLEGRO_KEY_N
case :
    break;
ALLEGRO_KEY_O
case :
    break;
ALLEGRO_KEY_P
case :
    break;
ALLEGRO_KEY_Q
case :
    break;
ALLEGRO_KEY_R
case :
    break;
ALLEGRO_KEY_S
case :
    break;
ALLEGRO_KEY_T
case :
    break;
ALLEGRO_KEY_U
case :
    break;
ALLEGRO_KEY_V
case :
    break;
ALLEGRO_KEY_W
case :
    break;
ALLEGRO_KEY_X
case :
    break;
ALLEGRO_KEY_Y
case :
    break;
ALLEGRO_KEY_Z
case :
    break;

ALLEGRO_KEY_0
case :
    break;
ALLEGRO_KEY_1
case :
    break;
ALLEGRO_KEY_2
case :
    break;
ALLEGRO_KEY_3
case :
    break;
ALLEGRO_KEY_4
case :
    break;
ALLEGRO_KEY_5
case :
    break;
ALLEGRO_KEY_6
case :
    break;
ALLEGRO_KEY_7
case :
    break;
ALLEGRO_KEY_8
case :
    break;
ALLEGRO_KEY_9
case :
    break;

ALLEGRO_KEY_PAD_0
case :
    break;
ALLEGRO_KEY_PAD_1
case :
    break;
ALLEGRO_KEY_PAD_2
case :
    break;
ALLEGRO_KEY_PAD_3
case :
    break;
ALLEGRO_KEY_PAD_4
case :
    break;
ALLEGRO_KEY_PAD_5
case :
    break;
ALLEGRO_KEY_PAD_6
case :
    break;
ALLEGRO_KEY_PAD_7
case :
    break;
ALLEGRO_KEY_PAD_8
case :
    break;
ALLEGRO_KEY_PAD_9
case :
    break;

ALLEGRO_KEY_F1
case :
    break;
ALLEGRO_KEY_F2
case :
    break;
ALLEGRO_KEY_F3
case :
    break;
ALLEGRO_KEY_F4
case :
    break;
ALLEGRO_KEY_F5
case :
    break;
ALLEGRO_KEY_F6
case :
    break;
ALLEGRO_KEY_F7
case :
    break;
ALLEGRO_KEY_F8
case :
    break;
ALLEGRO_KEY_F9
case :
    break;
ALLEGRO_KEY_F10
case :
    break;
ALLEGRO_KEY_F11
case :
    break;
ALLEGRO_KEY_F12
case :
    break;

ALLEGRO_KEY_ESCAPE
case :
    break;
ALLEGRO_KEY_TILDE
case :
    break;
ALLEGRO_KEY_MINUS
case :
    break;
ALLEGRO_KEY_EQUALS
case :
    break;
ALLEGRO_KEY_BACKSPACE
case :
    break;
ALLEGRO_KEY_TAB
case :
    break;
ALLEGRO_KEY_OPENBRACE
case :
    break;
ALLEGRO_KEY_CLOSEBRACE
case :
    break;
ALLEGRO_KEY_ENTER
case :
    break;
ALLEGRO_KEY_SEMICOLON
case :
    break;
ALLEGRO_KEY_QUOTE
case :
    break;
ALLEGRO_KEY_BACKSLASH
case :
    break;
ALLEGRO_KEY_BACKSLASH2
case :
    break;
ALLEGRO_KEY_COMMA
case :
    break;
ALLEGRO_KEY_FULLSTOP
case :
    break;
ALLEGRO_KEY_SLASH
case :
    break;
ALLEGRO_KEY_SPACE
case :
    break;
ALLEGRO_KEY_INSERT
case :
    break;
ALLEGRO_KEY_DELETE
case :
    break;
ALLEGRO_KEY_HOME
case :
    break;
ALLEGRO_KEY_END
case :
    break;
ALLEGRO_KEY_PGUP
case :
    break;
ALLEGRO_KEY_PGDN
case :
    break;
ALLEGRO_KEY_LEFT
case :
    break;
ALLEGRO_KEY_RIGHT
case :
    break;
ALLEGRO_KEY_UP
case :
    break;
ALLEGRO_KEY_DOWN
case :
    break;
ALLEGRO_KEY_PAD_SLASH
case :
    break;
ALLEGRO_KEY_PAD_ASTERISK
case :
    break;
ALLEGRO_KEY_PAD_MINUS
case :
    break;
ALLEGRO_KEY_PAD_PLUS
case :
    break;
ALLEGRO_KEY_PAD_DELETE
case :
    break;
ALLEGRO_KEY_PAD_ENTER
case :
    break;
ALLEGRO_KEY_PRINTSCREEN
case :
    break;
ALLEGRO_KEY_PAUSE
case :
    break;
ALLEGRO_KEY_ABNT_C1
case :
    break;
ALLEGRO_KEY_YEN
case :
    break;
ALLEGRO_KEY_KANA
case :
    break;
ALLEGRO_KEY_CONVERT
case :
    break;
ALLEGRO_KEY_NOCONVERT
case :
    break;
ALLEGRO_KEY_AT
case :
    break;
ALLEGRO_KEY_CIRCUMFLEX
case :
    break;
ALLEGRO_KEY_COLON2
case :
    break;
ALLEGRO_KEY_KANJI
case :
    break;
ALLEGRO_KEY_LSHIFT
case :
    break;
ALLEGRO_KEY_RSHIFT
case :
    break;
ALLEGRO_KEY_LCTRL
case :
    break;
ALLEGRO_KEY_RCTRL
case :
    break;
ALLEGRO_KEY_ALT
case :
    break;
ALLEGRO_KEY_ALTGR
case :
    break;
ALLEGRO_KEY_LWIN
case :
    break;
ALLEGRO_KEY_RWIN
case :
    break;
ALLEGRO_KEY_MENU
case :
    break;
ALLEGRO_KEY_SCROLLLOCK
case :
    break;
ALLEGRO_KEY_NUMLOCK
case :
    break;
ALLEGRO_KEY_CAPSLOCK
case :
    break;
ALLEGRO_KEY_PAD_EQUALS
case :
    break;
ALLEGRO_KEY_BACKQUOTE
case :
    break;
ALLEGRO_KEY_SEMICOLON2
case :
    break;
ALLEGRO_KEY_COMMAND
case :
    break;

ALLEGRO_KEYMOD_SHIFT
case :
    break;
ALLEGRO_KEYMOD_CTRL
case :
    break;
ALLEGRO_KEYMOD_ALT
case :
    break;
ALLEGRO_KEYMOD_LWIN
case :
    break;
ALLEGRO_KEYMOD_RWIN
case :
    break;
ALLEGRO_KEYMOD_MENU
case :
    break;
ALLEGRO_KEYMOD_ALTGR
case :
    break;
ALLEGRO_KEYMOD_COMMAND
case :
    break;
ALLEGRO_KEYMOD_SCROLLLOCK
case :
    break;
ALLEGRO_KEYMOD_NUMLOCK
case :
    break;
ALLEGRO_KEYMOD_CAPSLOCK
case :
    break;
ALLEGRO_KEYMOD_INALTSEQ
case :
    break;

case ALLEGRO_KEYMOD_ACCENT1:
    break;

case ALLEGRO_KEYMOD_ACCENT2:
    break;

case ALLEGRO_KEYMOD_ACCENT3:
    break;

case ALLEGRO_KEYMOD_ACCENT4:
    break;
}
*/
/*
ALLEGRO_KEY_A ... ALLEGRO_KEY_Z

ALLEGRO_KEY_0 ... ALLEGRO_KEY_9

ALLEGRO_KEY_PAD_0 ... ALLEGRO_KEY_PAD_9

ALLEGRO_KEY_F1 ... ALLEGRO_KEY_F12

ALLEGRO_KEY_ESCAPE
ALLEGRO_KEY_TILDE
ALLEGRO_KEY_MINUS
ALLEGRO_KEY_EQUALS
ALLEGRO_KEY_BACKSPACE
ALLEGRO_KEY_TAB
ALLEGRO_KEY_OPENBRACE
ALLEGRO_KEY_CLOSEBRACE
ALLEGRO_KEY_ENTER
ALLEGRO_KEY_SEMICOLON
ALLEGRO_KEY_QUOTE
ALLEGRO_KEY_BACKSLASH
ALLEGRO_KEY_BACKSLASH2
ALLEGRO_KEY_COMMA
ALLEGRO_KEY_FULLSTOP
ALLEGRO_KEY_SLASH
ALLEGRO_KEY_SPACE
ALLEGRO_KEY_INSERT
ALLEGRO_KEY_DELETE
ALLEGRO_KEY_HOME
ALLEGRO_KEY_END
ALLEGRO_KEY_PGUP
ALLEGRO_KEY_PGDN
ALLEGRO_KEY_LEFT
ALLEGRO_KEY_RIGHT
ALLEGRO_KEY_UP
ALLEGRO_KEY_DOWN
ALLEGRO_KEY_PAD_SLASH
ALLEGRO_KEY_PAD_ASTERISK
ALLEGRO_KEY_PAD_MINUS
ALLEGRO_KEY_PAD_PLUS
ALLEGRO_KEY_PAD_DELETE
ALLEGRO_KEY_PAD_ENTER
ALLEGRO_KEY_PRINTSCREEN
ALLEGRO_KEY_PAUSE
ALLEGRO_KEY_ABNT_C1
ALLEGRO_KEY_YEN
ALLEGRO_KEY_KANA
ALLEGRO_KEY_CONVERT
ALLEGRO_KEY_NOCONVERT
ALLEGRO_KEY_AT
ALLEGRO_KEY_CIRCUMFLEX
ALLEGRO_KEY_COLON2
ALLEGRO_KEY_KANJI
ALLEGRO_KEY_LSHIFT
ALLEGRO_KEY_RSHIFT
ALLEGRO_KEY_LCTRL
ALLEGRO_KEY_RCTRL
ALLEGRO_KEY_ALT
ALLEGRO_KEY_ALTGR
ALLEGRO_KEY_LWIN
ALLEGRO_KEY_RWIN
ALLEGRO_KEY_MENU
ALLEGRO_KEY_SCROLLLOCK
ALLEGRO_KEY_NUMLOCK
ALLEGRO_KEY_CAPSLOCK
ALLEGRO_KEY_PAD_EQUALS
ALLEGRO_KEY_BACKQUOTE
ALLEGRO_KEY_SEMICOLON2
ALLEGRO_KEY_COMMAND
ALLEGRO_KEYMOD_SHIFT
ALLEGRO_KEYMOD_CTRL
ALLEGRO_KEYMOD_ALT
ALLEGRO_KEYMOD_LWIN
ALLEGRO_KEYMOD_RWIN
ALLEGRO_KEYMOD_MENU
ALLEGRO_KEYMOD_ALTGR
ALLEGRO_KEYMOD_COMMAND
ALLEGRO_KEYMOD_SCROLLLOCK
ALLEGRO_KEYMOD_NUMLOCK
ALLEGRO_KEYMOD_CAPSLOCK
ALLEGRO_KEYMOD_INALTSEQ
ALLEGRO_KEYMOD_ACCENT1
ALLEGRO_KEYMOD_ACCENT2
ALLEGRO_KEYMOD_ACCENT3
ALLEGRO_KEYMOD_ACCENT4
*/
