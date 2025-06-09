/**
 * @file MusicBox.cpp
 * @author Jaylon Sifuentes
 */

#include "pch.h"
#include "MusicBox.h"

/// The music box mechanism image filename
const std::wstring MusicBoxImage = L"/images/mechanism.png";
/// Size to draw the music box mechanism image in pixels
const int MusicBoxImageSize = 120;
/// The color of the rotating music box drum
const wxColour MusicBoxDrumColor = wxColour(248, 242, 191);
/// The color of the lines on the music box rotating drum
const wxColour MusicBoxDrumLineColor = wxColour(20, 20, 20);
/// The music box drum width
const int MusicBoxDrumWidth = 40;
/// The music box drum diameter
const int MusicBoxDrumDiameter = 33;
/// Number of beats per turn of the music box drum
/// This also determines how many lines to draw on the cylinder
const int BeatsPerRotation = 12;
/// The audio directory in the resources
const std::wstring AudioDirectory = L"/audio/";
/// Divider applied to drum to slow down rotation appearance
const int DrumRotDiv = 4;
/// Drum X resize div
const double DrumXResize = 6.5;
/// Drum Y resize div
const double DrumYResize = 1.65;
/// Music box Y resize div
const int MusicBoxYResize = 10;
/// Number of lines for drum
const int DrumLineCount = 6;

MusicBox::MusicBox(std::wstring resourcesDir, std::wstring songXmlPath)
{
    mResourcesDir = resourcesDir;
    mMusicBoxImg.Rectangle(0, 0, MusicBoxImageSize, MusicBoxImageSize);
    mMusicBoxImg.SetImage(resourcesDir + MusicBoxImage);
    mDrumCylinder.SetSize(MusicBoxDrumDiameter, MusicBoxDrumWidth);
    mDrumCylinder.SetColour(MusicBoxDrumColor);
    mDrumCylinder.SetLines(MusicBoxDrumLineColor, 2, DrumLineCount);

    LoadXMLSong(resourcesDir + songXmlPath);
}

/**
 * Load all info from xml file and store to
 * use later to play song.
 */
void MusicBox::LoadXMLSong(std::wstring xmlPath)
{
    wxXmlDocument notesXml;
    notesXml.Load(xmlPath);
    wxXmlNode* root = notesXml.GetRoot();
    mBeatsPerMeasure = wxAtoi(root->GetAttribute("beats"));
    wxXmlNode* soundsSection = root->GetChildren();
    /*
     * Gather all the sounds and store them in a dict
     * with key value pair: note, sound file
     */
    while (soundsSection)
    {
        if (soundsSection->GetName() == "sounds")
        {
            wxXmlNode* soundLine = soundsSection->GetChildren();
            while (soundLine)
            {
                if (soundLine->GetName() == "sound")
                {
                    std::wstring note = soundLine->GetAttribute(L"note").ToStdWstring();
                    wxString file = mResourcesDir + AudioDirectory + soundLine->GetAttribute("file");
                    mSounds[note] = file;
                }
                soundLine = soundLine->GetNext();
            }
        }
        soundsSection = soundsSection->GetNext();
    }

    /*
     * Gather all the notes and store them
     * in a list of pairs: note, absolute beat.
     */
    wxXmlNode* notesSection = root->GetChildren();
    while (notesSection)
    {
        if (notesSection->GetName() == "notes")
        {
            wxXmlNode* noteLine = notesSection->GetChildren();
            while (noteLine)
            {
                if (noteLine->GetName() == "note")
                {
                    int measure = wxAtoi(noteLine->GetAttribute("measure"));
                    double beat = wxAtof(noteLine->GetAttribute("beat"));
                    std::wstring note = noteLine->GetAttribute("note").ToStdWstring();
                    // Absolute beat equation
                    double absoluteBeat = (measure - 1) * mBeatsPerMeasure + (beat - 1);
                    mNotes.push_back({note, absoluteBeat});
                }
                noteLine = noteLine->GetNext();
            }
        }
        notesSection = notesSection->GetNext();
    }
}

void MusicBox::PlayNote(std::wstring note)
{
    if (!mMuted)
    {
        wxSound sound(mSounds[note]);
        // async so program continues as normal
        sound.Play(wxSOUND_ASYNC);
    }
}


void MusicBox::UpdateRotation(double rotation)
{
    mRotation = rotation;
    // Calculate beat based on rotation
    double beat = rotation * mBeatsPerMeasure / 2;

    // When beat is greater than or equal to the next beat waiting to be played, play the beat
    while (mNoteIndex < mNotes.size() && beat >= mNotes[mNoteIndex].second)
    {
        PlayNote(mNotes[mNoteIndex].first);
        mNoteIndex++;
    }
}

void MusicBox::DrawComponentBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
    mMusicBoxImg.DrawPolygon(graphics, GetX() - MusicBoxImageSize / 2, GetY() - MusicBoxImageSize / MusicBoxYResize);
    mDrumCylinder.Draw(graphics, GetX() - MusicBoxImageSize / DrumXResize, GetY() - MusicBoxImageSize / DrumYResize, mRotation / DrumRotDiv);
}

void MusicBox::DrawComponentForeground(std::shared_ptr<wxGraphicsContext> graphics)
{
}

void MusicBox::Reset()
{
    mRotation = 0;
    mNoteIndex = 0;
}
