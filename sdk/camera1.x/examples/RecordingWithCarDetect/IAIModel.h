#ifndef IAIMODEL_H_
#define IAIMODEL_H_

#include <cstdint>

class IAIModel
{
public:
    // image size for SNPE model processing
    static const int MODEL_WIDTH = 300;
    static const int MODEL_HEIGHT = 300;

    // Origin (0,0): upper-left
    class Rect
    {
        uint32_t top;
        uint32_t left;
        uint32_t bottom;
        uint32_t right;

    public:
        Rect(uint32_t top, uint32_t left, uint32_t bottom, uint32_t right)
            : top(top), left(left), bottom(bottom), right(right) {}
        uint32_t getTop() const { return top; }
        uint32_t getLeft() const { return left; }
        uint32_t getBottom() const { return bottom; }
        uint32_t getRight() const { return right; }
        uint32_t getWidth() const { return right - left; }
        uint32_t getHeight() const { return bottom - top; }
    };

    /**
     * @brief Initialization.
     *
     * @return 0: succeed, -1: failed
     */
    virtual int init() = 0;

    /**
     * @brief Process a frame.
     *
     * @param data Video frame data in BGR format, size of the data should be "width * height * 3"
     * @param width Width of the frame
     * @param height Height of the frame
     * @param result Bounding box of the found object (if found)
     * @return  0: succeed but no object is found
     *         -1: failed
     *          1: succeed and an object is found, check "result" for the bounding box of the object
     */
    virtual int process(void *data, uint32_t width, uint32_t height, Rect &result) = 0;

    /**
     * @brief Deinitialization.
     *
     * @return 0: succeed, -1: failed
     */
    virtual int deinit() = 0;
};

#endif /* IAIMODEL_H_ */
