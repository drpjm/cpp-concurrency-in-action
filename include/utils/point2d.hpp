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
};

#endif // POINT2D_QUEUE_HPP