/**
 * @file IKeyResponder.h
 * @author Jaylon Sifuentes
 *
 * This class is an interface that talks to Box and Sparty,
 * letting them know the state of the key.
 */

#ifndef IKEYRESPONDER_H
#define IKEYRESPONDER_H


/**
 * Interface class for handling the interaction between cam, Sparty, and Box.
 * The cam notifies the interface of the key drop, which triggers those who
 * implement it (Sparty and Box).
 */
class IKeyResponder
{
private:

public:
    /**
     * Triggered by cam when the key falls into the fall, which
     * notifies Box and Sparty's respective implementations.
     */
    virtual void OnKeyDrop() = 0;
};


#endif //IKEYRESPONDER_H
