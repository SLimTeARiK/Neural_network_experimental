struct empty_button{

    float x = 0.0f;  // Положение
    float y = 0.0f;
    float x1 = 0.0f;  // Координата верхнего левого угла
    float y1 = 0.0f;
    float x2 = 20.0f;  // Координата нижнего правого угла
    float y2 = 20.0f;
    ALLEGRO_COLOR color  = al_map_rgb(125, 125, 125);
    //ALLEGRO_COLOR color_frame  = al_map_rgb(100, 100, 100);

    bool mouse_inside_bounds = false;


    empty_button() {

    }
    empty_button(float new_x, float new_y, float new_x1, float new_y1, float new_x2, float new_y2, ALLEGRO_COLOR new_color){
        x = new_x;  // Координата верхнего левого угла
        y = new_y;
        x1 = new_x1;  // Координата верхнего левого угла
        y1 = new_y1;
        x2 = new_x2;  // Координата нижнего правого угла
        y2 = new_y2;
        color = new_color;
    }

    void set_position(float new_x, float new_y){
        x = new_x;  // Координата верхнего левого угла
        y = new_y;
    }

    void set_size(float new_x1, float new_y1, float new_x2, float new_y2){
        x1 = new_x1;  // Координата верхнего левого угла
        y1 = new_y1;
        x2 = new_x2;  // Координата нижнего правого угла
        y2 = new_y2;
    }

    void set_color(ALLEGRO_COLOR color){
        color = color;
    }

    bool check_mouse_inside_bounds(float nx, float ny) const {
    if((ny > y+y1 && ny < y+y2)&&
       (nx > x+x1 && nx < x+x2))
    return true;
    else return false;
    }



    public: void draw() const{
        al_draw_filled_rectangle(x1+x, y1+y, x2+x, y2+y, color);
        //al_draw_rectangle(x1+x, y1+y, x2+x, y2+y, color_frame, 1);
    }
};

struct box_button{
//int x;400
//int y;350
private:
float x1 = 20.0f;  // Координаты верхнего левого угла
float y1 = 50.0f;
float x2 = 740.0f;  // Координаты нижнего правого угла
float y2 = 730.0f;
float radius = 10.0f;  // Радиус скругления углов
ALLEGRO_COLOR color  = al_map_rgb(125, 125, 125);
ALLEGRO_COLOR frame_color  = al_map_rgb(100, 100, 100);


public: void draw() const{
al_draw_filled_rounded_rectangle(x1, y1, x2, y2, radius, radius, color);
al_draw_rounded_rectangle(x1, y1, x2, y2, radius, radius, frame_color, 4.0f);
}




};

struct buttons_from_file_data{

    std::ifstream ifile;
    std::ofstream ofile;
    buttons_from_file_data(){
    }

void* create_buttons_from_file_data(const char path[]){
    ifile = std::ifstream(path, std::ios::binary | std::ios::app);
    ofile = std::ofstream(path, std::ios::binary | std::ios::app);
    if (!ifile.is_open()) {
        std::cerr << "not open ifstream ../res/buttons/button.bin";throw std::runtime_error("not open ifstream ../res/buttons/button.bin");
        return nullptr;
    }
    if (!ofile.is_open()) {
        std::cerr << "not open ofstream ../res/buttons/button.bin";throw std::runtime_error("not open ofstream ../res/buttons/button.bin");
        return nullptr;
    }
}
void* change_buttons_from_file_data(int bytes_to_read){

    char *buffer = new char[bytes_to_read];
    ifile.read(buffer, bytes_to_read);


    ofile.write(buffer, bytes_to_read);
    delete[] buffer;
    return nullptr;
}
};


typedef enum ButtonBitMask{
ROUNDING_BUTTON,
COLOR_BUTTON,
MOVING_MOUSE_BUTTON,
CLICK_MOUSE_BUTTON,


};










