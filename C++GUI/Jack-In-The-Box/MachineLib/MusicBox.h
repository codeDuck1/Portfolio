/**
 * @file MusicBox.h
 * @author Jaylon Sifuentes
 *
 * Class that represents the music box.
 */

#ifndef MUSICBOX_H
#define MUSICBOX_H
#include <map>
#include <wx/sound.h>
#include <wx/xml/xml.h>
#include "Component.h"
#include "Cylinder.h"
#include "IRotationSink.h"
#include "Polygon.h"

/**
 * Objects of this class are a music box that play
 * music according to a XML file with notes and .wav files.
 */
class MusicBox : public Component, public IRotationSink
{
private:
    /// Rotation received from source
    double mRotation = 0;
    /// Resources directory. Seperate variable for XML loading .wav's
    std::wstring mResourcesDir;
    /// Beats per measure at top of XML
    int mBeatsPerMeasure = 0;
    /// If the box is muted
    bool mMuted = false;
    /// Dictionary that stores all notes and their corresponding sound files
    std::map<std::wstring, wxString> mSounds;
    /// List that stores notes and corresponding beat calculation
    std::vector<std::pair<std::wstring, double>> mNotes;
    /**
     * Index increased during rotation.
     * Determines note to play from mNotes
     */
    int mNoteIndex = 0;
    /// Image for drawing music box
    cse335::Polygon mMusicBoxImg;
    /// Image for drawing music box rotating cylinder
    cse335::Cylinder mDrumCylinder;

public:
    /**
     * Music Box constructor
     * Setup the music box image and load given XML song
     * @param resourcesDir resources direct
     * @param songXmlPath path to song
     */
    MusicBox(std::wstring resourcesDir, std::wstring songXmlPath);
    /**
    * Draw the component at the currently specified location in the background.
    * @param graphics Graphics object to render to
    */
    void DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics) override;
    /**
    * Draw the component at the currently specified location in the background.
    * @param graphics Graphics object to render to
    */
    void DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics) override;
    /**
    * Reset Music box attributes
    */
    void Reset() override;
    /**
     * Mute the music box
     * @param mute if the music box should be muted
     */
    void Mute(bool mute) { mMuted = mute; }
    /**
     * Updates the rotation of the music box based on its source.
     * Used to progress song.
     * Override from IRotationSink
     * @param rotation Amount of rotation to be applied
     */
    void UpdateRotation(double rotation) override;
    /**
     * Loads XML song info and stores in dict+list
     * to make it playable.
     * @param xmlPath xml file path for song
     */
    void LoadXMLSong(std::wstring xmlPath);
    /**
     * Plays a given note's respective .wav
     * @param note note to play
     */
    void PlayNote(std::wstring note);
};


#endif //MUSICBOX_H
