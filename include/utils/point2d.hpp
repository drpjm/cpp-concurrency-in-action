#ifndef POINT2D_QUEUE_HPP
#define POINT2D_QUEUE_HPP


class Point2D
{
    float x_pos_;
    float y_pos_;
public:
    Point2D(float x, float y) : x_pos_ (x), y_pos_ (y) {};
 
    float get_x() const {
        return x_pos_;
    }

    float get_y() const {
        return y_pos_;
    }

    void set_x(float x) {
        x_pos_ = x;
    }

    void set_y(float y) {
        y_pos_ = y;
    }

};

#endif // POINT2D_QUEUE_HPP