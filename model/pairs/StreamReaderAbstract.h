#ifndef STREAMREADERABSTRACT_H
#define STREAMREADERABSTRACT_H

class StreamReaderAbstract
{
public:
    virtual void readStream() = 0;
    virtual void stopReadStream() = 0;
};

#endif // STREAMREADERABSTRACT_H
