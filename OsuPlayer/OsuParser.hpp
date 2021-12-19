/*****************************************************************//**
 * \file   OsuParser.hpp
 * \brief  A simple header-only library for parsing osu file
 * 
 * \author peter
 * \date   September 2021
 *********************************************************************/
#pragma once
#include <fstream>
#include <filesystem>
#include <exception>
#include <vector>
#include <string>
#include <sstream>
#include <string_view>
#include <array>
#include <optional>
#include <iostream>
#include <cmath> //pow() on GCC need this header
#include <iterator> //std::istream_iterator on GCC need this header
#include <algorithm>//std::clamp
#include <numeric>
#include <variant>
#include <cassert>
#include <iterator>
#include <cstring>

//#include <queue>

/**
 * @brief Osu editor play field size
 */
namespace PlayField
{
    constexpr int xMax = 512;
    constexpr int yMax = 384;
    
    constexpr inline std::string_view xMid = "256";
    constexpr inline std::string_view yMid = "192";
}


namespace details 
{
    //template<typename T>
    //class SortedArrayMerge
    //{
    //    std::priority_queue<T*> queue;

    //};

    /**
     * @brief Remove spacing or specified characters of both side of the string
     * @param s The source string to be trimmed
     * @param contentToTrim String of characters that needs to be removed from the source string
     */
    static inline auto TrimStr(std::string& s, const char* contentToTrim = "\t\n\v\f\r ")
    {
        s.erase(0, s.find_first_not_of(contentToTrim)); // left trim
        s.erase(s.find_last_not_of(contentToTrim) + 1); // right trim
    }

    /**
     * @brief Remove specing of specified characters of both side of the string and return the new string
     * @param s The source string
     * @param contentToTrim String of characters that needs to be removed from the source string
     * @return The new trimmed string
     */
    static inline auto TrimStr(std::string_view s, const char* contentToTrim = "\t\n\v\f\r ")
    {
        auto const start = s.find_first_not_of(contentToTrim);
        auto const end = s.find_last_not_of(contentToTrim);
        return s.substr(start, end - start + 1);
    }

    /**
     * @brief Split a string representing a [key, value] pair
     * @param line The source string
     * @param separator The separator of key and value
     * @return a pair of [key, value], with whitespace trimmed
     */
    static inline auto SplitKeyVal(std::string_view line, char separator = ':')
    {
        if (auto const splitPos = line.find(separator); splitPos != std::string::npos)
        {
            auto const valuePos = line.find_first_not_of(" ", splitPos + 1);
            return std::pair{
                splitPos == 0 ? "" : line.substr(0, line.find_last_not_of(" ", splitPos - 1) + 1),
                valuePos != std::string::npos ? line.substr(valuePos) : ""
            };
        }

        return std::pair<std::string_view, std::string_view>{"", ""};
    }

    /**
     * @brief Split a space separated string into individual elements (usually words)
     * @tparam T the element type of the vector
     * @return A vector of splited element
     */
    template<typename T = std::string>
    static inline auto SplitWords(std::string_view s)
    {
        std::stringstream ss{ s.data() };
        return std::vector(std::istream_iterator<T>{ss}, std::istream_iterator<T>{});
    }

    /**
     * @brief Split a comma separated string
     * @return Splited inidividual sub-strings
     */
    static inline auto SplitCommaSeparatedString(std::string_view s)
    {
        std::vector<int> result;
        std::stringstream ss{ s.data() };

        for (int i{}; ss >> i; )
        {
            result.push_back(i);
            if (ss.peek() == ',' || ss.peek() == ' ')
                ss.ignore();
        }

        return result;
    }

    /**
     * @brief Split a fixed number of separated string
     * @details If the the split result array more needed, and the `appendRest` parameter is true, the last element will contain the rest of the string
     * Otherwise, the last element contains only the sub-string part
     * If the split result is less than required, the rest element are empty strings
     */
    template<size_t N>
    static inline auto SplitString(std::string_view str, char delim = ',', bool appendRest = false)
    {
        std::array<std::string_view, N> result;
        size_t startPos = 0, endPos = str.find(delim);
        size_t i = 0;
        while (endPos != std::string::npos && i < N)
        {
            if (appendRest && i + 1 == N)
                result[i] = str.substr(startPos);
            else
                result[i] = str.substr(startPos, endPos - startPos);
            startPos = endPos + 1;
            endPos = str.find(delim, startPos);
            ++i;
        }
        for (; i < N; ++i)
        {
            if (startPos > str.size() - 1)
                result[i] = "";
            else
            {
                result[i] = str.substr(startPos);
                startPos = std::string::npos;
            }
        }
        return result;
    }

    /**
     * @brief 
     */
    static inline auto SplitString(std::string_view str, char delim = ',')
    {
        std::vector<std::string_view> result;
        size_t startPos = 0, endPos = str.find(delim);
        while (endPos != std::string::npos)
        {
            result.push_back(str.substr(startPos, endPos - startPos));
            startPos = endPos + 1;
            endPos = str.find(delim, startPos);
        }
        result.push_back(str.substr(startPos));
        return result;
    }

    /**
     * @brief Determines whether the string is empty or containing nothing
     */
    static inline auto IsEmptyLine(std::string const& line)
    {
        return line.empty() || line == "";
    }

    /**
     * @brief Get a line that is not empty nor starting with "//"
     * @param indicateEmptyLine Return false when the line is empty
     * @param file The input file stream
     * @param line The string to store the line
     * @return Return false when the file has reached the end, 
     * additionally return false when `indicateEmptyLine` is true and the file has reached an empty line.
     */
    static inline auto GetLine(std::ifstream& file, std::string& line, bool indicateEmptyLine = true)
    {
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r')
                line = line.substr(0, line.size() - 1);


            if (line.empty())
            {
                if (indicateEmptyLine)
                    return false;
                else
                    continue;
            }

            if (line[0] == '/' && line[1] == '/')
                continue;

            else if (auto const pos = line.find("//") != std::string::npos)
                line = line.substr(0, pos);
            return true;
        }
        return indicateEmptyLine ? false : static_cast<bool>(file);
    }

    /**
     * @brief Keep reading the file line by line until a target string is found
     * @param target A line to be found
     * @param line The input string, and when the `target` is found, the line should contain the `target` string
     * @param file The input file stream
     */
    static void SkipUntil(std::string_view target, std::string& line, std::ifstream& file)
    {
        do {
            if(!GetLine(file, line, false))
                break;
        } while (line != target);
    }

    /**
     * @brief Remove specific characters in a string and returns a new string
     * @param src The source string
     * @param contents All the characters that needs to be removed from the string, default values are "\\/:*?\"<>|"
     * @details This function is used in case there are invalid characters on Windows as file name
     */
    static auto RemoveInStr(std::string_view src, char const* content="\\/:*?\"<>|")
    {
        std::string s{ src };
        s.erase(std::remove_if(s.begin(), s.end(), [content, count = strlen(content)](char c){ return std::find(content, content + count, c) != content + count; }), s.end());
        return s;
    }

    /**
     * @brief A helper class to wrap calls to `std::ostream::operator<<`
     */
    class PrintHelper
    {
        std::ostream& os;
    public:
        PrintHelper(std::ostream& os) : os{ os } {}

        template<typename T>
        PrintHelper& print(std::vector<T> const& vec, char const* delim)
        {
            std::copy(vec.cbegin(), vec.cend() - 1, std::ostream_iterator<T>{os, delim});
            if (!vec.empty())   os << vec.back();
            return *this;
        }

        template<typename T>
        PrintHelper& print(T&& arg)
        {
            os << arg;
            return *this;
        }

        template<typename ...Args>
        PrintHelper& printLn(Args&&... args)
        {
            ((os << args), ...) << '\n';
            return *this;
        }

        template<typename Object>
        PrintHelper& printLn(std::vector<std::unique_ptr<Object>> const& objects)
        {
            for (auto const& ptr : objects)
                os << *ptr << '\n';
            return *this;
        }

        template<typename T>
        PrintHelper& printLn(std::vector<T> const& objects)
        {
            for (auto const& obj : objects)
                os << obj << '\n';
            return *this;
        }

        template<typename ...Args>
        PrintHelper& printLn(char delim = ',', Args&&... args)
        {
            ((os << args << delim), ...) << '\n';
            return *this;
        }

        template<typename Key, typename Element>
        PrintHelper& printLn(Key&& key, std::vector<Element> const& vec, char const* const delim = ",")
        {
            os << key;
            std::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<Element>{os, delim});
            os << '\n';
            return *this;
        }

        template<typename Key, typename Value>
        PrintHelper& printIfValueNotEmpty(Key&& key, Value&& value)
        {
            if (value.empty())
                return *this;
            else
                return printLn(key, value);
        }

        template<typename Key, typename OptionalValue>
        PrintHelper& printIfValueNotEmpty(Key&& key, std::optional<OptionalValue> const& value)
        {
            if (value.has_value())
                return printLn(key, *value);
            else return *this;
        }
    };
}


enum class Countdown : int
{
    No = 0,
    Normal = 1,
    Half = 2,
    Double = 3
};



enum class SampleSet
{
    Auto,
    Normal,
    Soft,
    Drum
};

inline std::ostream& operator<<(std::ostream& os, SampleSet sampleSet)
{
    //switch (sampleSet)
    //{
    //    case SampleSet::Auto:
    //        os << "Auto";
    //        break;
    //    case SampleSet::Normal:
    //        os << "Normal";
    //        break;
    //    case SampleSet::Soft:
    //        os << "Soft";
    //        break;
    //    case SampleSet::Drum:
    //        os << "Drum";
    //        break;
    //    default:
    //        break;
    //}
    os << static_cast<int>(sampleSet);
    return os;
}


enum class Mode
{
    Osu = 0,
    Taiko = 1,
    Catch = 2,
    Mania = 3
};

inline std::ostream& operator<<(std::ostream& os, Mode mode)
{
    os << static_cast<int>(mode);
    return os;
}

enum class OverlayPosition
{
    NoChange,
    Below,
    Above
};

inline std::ostream& operator<<(std::ostream& os, OverlayPosition overlayPosition)
{
    switch (overlayPosition)
    {
        case OverlayPosition::NoChange:
            os << "NoChange";
            break;
        case OverlayPosition::Below:
            os << "Below";
            break;
        case OverlayPosition::Above:
            os << "Above";
            break;
        default:
            break;
    }
    return os;
}

/**
 * @brief General information about the originalBeatmap
 * @see https://github.com/ppy/osu-wiki/blob/master/wiki/osu!_File_Formats/Osu_(file_format)/en.md#general
 */
struct General
{
    /**
     * @brief Location of the audio file relative to the current folder
     */
    std::string audioFile;

    /**
     * @brief Milliseconds of silence before the audio starts playing
     */
    int audioLeanIn = 0;

    /**
     * @brief Time in milliseconds when the audio preview should start
     */
    int previewTime = -1;

    /**
     * @brief Speed of the countdown before the first hit object
     */
    Countdown countdown = Countdown::Normal;

    /**
     * @brief Sample set that will be used if timing points do not override it (Normal, Soft, Drum)
     */
    SampleSet sampleSet = SampleSet::Normal;

    /**
     * @brief Multiplier for the threshold in time where hit objects placed close together stack (0–1)
     */
    float stackLeniency = 0.7f;

    /**
     * @brief Game mode
     */
    Mode mode = Mode::Osu;

    /**
     * @brief Whether or not breaks have a letterboxing effect
     */
    bool letterboxInBreaks = false;

    /**
     * @brief Whether or not the storyboard can use the user's skin images
     */
    bool useSkinSprites = false;

    /**
     * @brief Draw order of hit circle overlays compared to hit numbers
     */
    OverlayPosition overlayPosition = OverlayPosition::NoChange;

    /**
     * @brief Preferred skin to use during gameplay
     */
    std::string skinPreference;

    /**
     * @brief Whether or not a warning about flashing colours should be shown at the beginning of the map
     */
    bool epilepsyWarning = false;

    /**
     * @brief Time in beats that the countdown starts before the first hig object
     */
    int countdownOffset = 0;

    /**
     * @brief Whether or not the "N+1" style key layout is used for osu!mania
     */
    bool specialStyle = false;

    /**
     * @brief Whether or not the storyboard allows widescreen viewing
     */
    bool wideScreenStoryboard = false;

    /**
     * @brief Whether or not sound samples will change rate when playing with speed-changing mods
     */
    bool samplesMatchPlaybackRate = 0;

    /**
     * @brief Parse General info from osu file
     * @param file The input file stream
     * @param partial Whether this is a partial parse
     */
    General(std::ifstream& file, bool partial = true)
    {
        std::string line;
        
        /*If this is a partial parse, get line until [General] appears */
        if (partial)
            details::SkipUntil("[General]", line, file);

        while (details::GetLine(file, line))
        {
            auto [key, value] = details::SplitKeyVal(line);

            if (key == "AudioFilename")  audioFile = value;
            else if (key == "AudioLeadIn") audioLeanIn = std::stoi(value.data());
            else if (key == "PreviewTime") previewTime = std::stoi(value.data());
            else if (key == "Countdown") countdown = static_cast<Countdown>(std::stoi(value.data()));
            else if (key == "SampleSet")
            {
                if (value == "Soft") sampleSet = SampleSet::Soft;
                else if (value == "Normal") sampleSet = SampleSet::Normal;
                else if (value == "Drum") sampleSet = SampleSet::Drum;
            }
            else if (key == "StackLeniency") stackLeniency = std::stof(value.data());
            else if (key == "Mode") mode = static_cast<Mode>(std::stoi(value.data()));
            else if (key == "LetterboxInBreaks") letterboxInBreaks = std::stoi(value.data());
            else if (key == "UseSkinSprites") useSkinSprites = std::stoi(value.data());
            else if (key == "OverlayPosition")
            {
                if (value == "NoChange") overlayPosition = OverlayPosition::NoChange;
                else if (value == "Below") overlayPosition = OverlayPosition::Below;
                else if (value == "Above") overlayPosition = OverlayPosition::Above;
            }
            else if (key == "SkinPreference") skinPreference = value;
            else if (key == "EpilepsyWarning") epilepsyWarning = std::stoi(value.data());
            else if (key == "CountdownOffset") countdownOffset = std::stoi(value.data());
            else if (key == "SpecialStyle") specialStyle = std::stoi(value.data());
            else if (key == "WidescreenStoryboard") wideScreenStoryboard = std::stoi(value.data());
            else if (key == "SamplesMatchPlaybackRate") samplesMatchPlaybackRate = std::stoi(value.data());
        }
    }

    General() = default;

    friend std::ostream& operator<<(std::ostream& os, General const& general)
    {
        details::PrintHelper helper{ os };
        helper.printLn("[General]")
            .printLn("AudioFilename: ", general.audioFile)
            .printLn("AudioLeadIn: ", general.audioLeanIn)
            .printLn("PreviewTime: ", general.previewTime)
            .printLn("Countdown: ", static_cast<int>(general.countdown))
            .printLn("SampleSet: ", general.sampleSet)
            .printLn("StackLeniency: ", general.stackLeniency)
            .printLn("Mode: ", general.mode)
            .printLn("LetterboxInBreaks: ", general.letterboxInBreaks)
            .printLn("UseSkinSprites: ", general.useSkinSprites)
            .printLn("OverlayPosition: ", general.overlayPosition)
            .printIfValueNotEmpty("SkinPreference: ", general.skinPreference)
            .printLn("EpilepsyWarning: ", general.epilepsyWarning)
            .printLn("CountdownOffset: ", general.countdownOffset);
        if (general.mode == Mode::Mania) helper.printLn("SpecialStyle: ", general.specialStyle);
        helper.printLn("WidescreenStoryboard: ", general.wideScreenStoryboard)
            .printLn("SamplesMatchPlaybackRate: ", general.samplesMatchPlaybackRate);
        return os;
    }
};

/**
 * @brief Saved settings for the originalBeatmap editor
 */
struct Editor
{
    /**
     * @brief Time in milliseconds of bookmarks
     */
    std::vector<int> bookmarks;

    /**
     * @brief Distance snap multiplier
     */
    float distanceSpacing{};

    /**
     * @brief Beat snap divisor
     */
    float beatDivisor{};

    /**
     * @brief Grid size
     */
    int gridSize{};

    /**
     * @brief Scale factor for the object timeline
     */
    std::optional<float> timelineZoom;

    /**
     * @brief Parse Editor info from osu file
     * @param file The input file stream
     * @param partial Whether this is a partial parse
     */
    Editor(std::ifstream& file, bool partial = true)
    {
        std::string line;

        /*If this is a partial parse, get line until [Editor] appears*/
        if (partial)
            details::SkipUntil("[Editor]", line, file);


        while(details::GetLine(file, line))
        {
            auto [key, value] = details::SplitKeyVal(line);
            
            if (key == "Bookmarks")             bookmarks = details::SplitCommaSeparatedString(value);
            else if (key == "DistanceSpacing")  distanceSpacing = std::stof(value.data());
            else if (key == "BeatDivisor")      beatDivisor = std::stof(value.data());
            else if (key == "GridSize")         gridSize = std::stoi(value.data());
            else if (key == "TimelineZoom")     timelineZoom = std::stof(value.data());
        }
    }

    Editor() = default;

    friend std::ostream& operator<<(std::ostream & os, Editor const& editor)
    {
        details::PrintHelper{ os }
            .printLn("[Editor]")
            .printLn("Bookmarks: ", editor.bookmarks)
            .printLn("DistanceSpacing: ", editor.distanceSpacing)
            .printLn("BeatDivisor: ", editor.beatDivisor)
            .printIfValueNotEmpty("TimelineZoom: ", editor.timelineZoom);
        return os;
    }
};



/**
 * @brief Difficulty settings
 */
struct Difficulty
{
    /**
     * @brief HP setting (0–10)
     */
    float HPDrainRate{};

    /**
     * @brief CS setting (0–10)
     * @note CS determines the number of columns in osu!mania mode.
     */
    float circleSize{};

    /**
     * @brief OD setting (0–10)
     */
    float overallDifficulty{};

    /**
     * @brief AR setting (0–10)
     */
    float approachRate{};

    /**
     * @brief Base slider velocity in hecto-osu! pixels per beat
     */
    float sliderMultiplier{};

    /**
     * @brief Amount of slider ticks per beat
     */
    int sliderTickRate{};

    /**
     * @brief Parse Difficulty info from osu file
     * @param file The input file stream
     * @param partial Whether this is a partial parse
     */
    Difficulty(std::ifstream& file, bool partial = true)
    {
        std::string line;

        /*If this is a partial parse, skip until [Difficulty] appears*/
        if (partial)
            details::SkipUntil("[Difficulty]", line, file);

        while (details::GetLine(file, line))
        {
            auto [key, value] = details::SplitKeyVal(line);

            if (key == "HPDrainRate") HPDrainRate = std::stof(value.data());
            else if (key == "CircleSize") circleSize = std::stof(value.data());
            else if (key == "OverallDifficulty") overallDifficulty = std::stof(value.data());
            else if (key == "ApproachRate") approachRate = std::stof(value.data());
            else if (key == "SliderMultiplier") sliderMultiplier = std::stof(value.data());
            else if (key == "SliderTickRate") sliderTickRate = std::stoi(value.data());
        }
    }

    Difficulty() = default;

    friend std::ostream& operator<<(std::ostream& os, Difficulty const& difficulty)
    {
        details::PrintHelper{ os }
            .printLn("[Difficulty]")
            .printLn("HPDrainRate: ", difficulty.HPDrainRate)
            .printLn("CircleSize: ", difficulty.circleSize)
            .printLn("OverallDifficulty: ", difficulty.overallDifficulty)
            .printLn("ApproachRate: ", difficulty.approachRate)
            .printLn("SliderMultiplier: ", difficulty.sliderMultiplier)
            .printLn("SliderTickRate: ", difficulty.sliderTickRate);
        return os;
    }
};

struct Coord
{
    int x{}, y{};

    /**
     * @brief Calculate the distance to another hit object
     * @param anotherObject the other hit object
     */
    [[nodiscard]] auto distanceTo(Coord another) const
    {
        return sqrt(pow(x - another.x, 2) + pow(y - another.y, 2));
    }

    constexpr bool operator==(Coord rhs) const { return x == rhs.x && y == rhs.y; }

    friend std::ostream& operator<<(std::ostream& os, Coord coord)
    {
        os << coord.x << ':' << coord.y;
        return os;
    }
};

struct TimingPoint
{
    enum class Effect
    {
        Kiai = 1,
        OmitBarline = 1 << 3
    };

    enum class Type
    {
        TimingControlPoint,
        DifficultyControlPoint,
        SampleControlPoint,
        EffectControlPoint,
        All
    };

    /**
     * @brief Start time of the timing section, in milliseconds from the beginning of the originalBeatmap's audio.
     * The end of the timing section is the next timing point's time (or never, if this is the last timing point).
     */
    int time{};

    /**
     * @brief For uninherited timing points, the duration of a beat, in milliseconds.
     * For inherited timing points, a negative inverse slider velocity multiplier, as a percentage.
     *
     * @example For example, -50 would make all sliders in this timing section twice as fast as SliderMultiplier.
     */
    float beatLength{};

    /**
     * @brief Amount of beats in a measure. Inherited timing points ignore this property.
     */
    int meter{};

    /**
     * @brief Default sample set for hit objects (0 = originalBeatmap default, 1 = normal, 2 = soft, 3 = drum).
     */
    SampleSet sampleSet{};

    /**
     * @brief Custom sample index for hit objects. 0 indicates osu!'s default hitsounds.
     */
    int sampleIndex{};

    /**
     * @brief Volume percentage for hit objects.
     */
    int volume{};

    /**
     * @brief Whether or not the timing point is uninherited.
     */
    bool uninherited{};

    /**
     * @brief Bit flags that give the timing point extra effects.
     */
    unsigned effects{};

    TimingPoint() = default;

    TimingPoint(std::string_view line) : TimingPoint(details::SplitString<8>(line))
    {
    }

    /**
     * @details Timing point syntax:
     *      time,beatLength,meter,sampleSet,sampleIndex,volume,uninherited,effects
     */
    TimingPoint(std::array<std::string_view, 8> const& result)
        : time{ std::stoi(result[0].data()) },
        beatLength{ std::stof(result[1].data()) },
        meter{ std::stoi(result[2].data()) },
        sampleSet{ static_cast<SampleSet>(std::stoi(result[3].data())) },
        sampleIndex{ std::stoi(result[4].data()) },
        volume{ std::stoi(result[5].data()) },
        uninherited{ static_cast<bool>(std::stoi(result[6].data())) },
        effects{ static_cast<unsigned>(std::stoul(result[7].data())) }
    {
    }

    /**
     * @brief Parse timing points
     * @param file The `.osu` file stream
     * @param partial Whether this is a partial parse. If true, it skips until the line "[TimingPoints]" is encountered. 
     * Otherwise, it tries to parse from the current line.
     * @return A `std::vector<TimingPoint>`
     */
    static auto HandleTimingPoints(std::ifstream& file, bool partial = true)
    {
        std::vector<TimingPoint> timingPoints;
        std::string line;

        if (partial)
            details::SkipUntil("[TimingPoints]", line, file);

        while (details::GetLine(file, line))
        {
            timingPoints.emplace_back(line);
        }
        return timingPoints;
    }

    template<Type type>
    [[nodiscard]] constexpr bool isType() const
    {
        if constexpr (type == Type::All)
            return true;
        else if constexpr (type == Type::TimingControlPoint)
            return uninherited;
    }

    friend std::ostream& operator<<(std::ostream& os, TimingPoint const& timingPoint)
    {
        os << timingPoint.time << ','
            << timingPoint.beatLength << ','
            << timingPoint.meter << ','
            << static_cast<int>(timingPoint.sampleSet) << ','
            << timingPoint.sampleIndex << ','
            << timingPoint.volume << ','
            << timingPoint.uninherited << ','
            << timingPoint.effects;
        return os;
    }
};


struct Circle;
struct Slider;
struct Spinner;
struct Hold;

/**
 * @brief Base class of all hit objects in osu
 * @details Hit object syntax: 
 *      x,y,time,type,hitSound,objectParams,hitSample
 *      168,326,620,5,0,1:0:0:0:
*/
struct HitObject
{
    enum class HitSound
    {
        Normal = 1,
        Whistle = 1 << 1,
        Finish = 1 << 2,
        Clap = 1 << 3
    };

    /**
     * @brief Position in osu!pixels (https://osu.ppy.sh/wiki/zh-hk/osupixel) of the object
     */
    int x;

    /**
     * @brief Position in osu!pixels (https://osu.ppy.sh/wiki/zh-hk/osupixel) of the object
     */
    int y;

    /**
     * @brief Time when the object is to be hit, in milliseconds from the beginning of the originalBeatmap's audio.
     */
    int time;

    /**
     * @brief Indicating the hitsound applied to the object
     */
    HitSound hitSound;

    /**
     * @brief Whether this hit object is the start of a combo.
     */
    bool isNewCombo = false;

    struct HitSample
    {
        SampleSet normalSet;
        SampleSet additionSet;
        int index;
        int volume;
        std::string filename;

        HitSample() : normalSet{ 0 }, additionSet{ 0 }, index{ 0 }, volume{ 0 } {}

        HitSample(int normalSet, int additionSet, int index, int volume, std::string filename="")
            :normalSet{normalSet},
            additionSet{additionSet},
            index{index},
            volume{ volume },
            filename{ std::move(filename) }
        {
        }

        HitSample(std::string_view colonSeparatedList) : HitSample()
        {
            //Older version allow this to be completely absent
            if (!colonSeparatedList.empty())
            {
                auto const result = details::SplitString<5>(colonSeparatedList, ':');

                normalSet = static_cast<SampleSet>(std::stoi(result[0].data()));
                additionSet = static_cast<SampleSet>(std::stoi(result[1].data()));
                index = result[2].empty() ? 0 : std::stoi(result[2].data());
                volume = result[3].empty() ? 0 : std::stoi(result[3].data());
                filename = result[4].empty() ? "" : result[4];
            }
        }

        [[nodiscard]] constexpr bool has(SampleSet sampleSet) const
        {
            return normalSet == sampleSet || additionSet == sampleSet;
        }

        friend std::ostream& operator<<(std::ostream& os, HitSample const& hitSample)
        {
            os <<  hitSample.normalSet << ':'
                << hitSample.additionSet << ':'
                << hitSample.index << ':'
                << hitSample.volume << ':'
                << hitSample.filename;
            return os;
        }
    };

    HitSample hitSample;

protected:
    /*
    Some special bits representing hit object type
     [type] is an 8-bit integer:
         1 Circle
         2 Slider
         8 Spinner
         *bit[2] set -> new combo
    */
    constexpr static inline unsigned CircleBit = 0b1;
    constexpr static inline unsigned SliderBit = 0b1 << 1;
    constexpr static inline unsigned ComboBit = 0b1 << 2;
    constexpr static inline unsigned SpinnerBit = 0b1 << 3;
    constexpr static inline unsigned HoldBit = 0b1 << 7;

public:
    enum class Type
    {
        Circle = CircleBit,
        Slider = SliderBit,
        Spinner = SpinnerBit,
        Hold = HoldBit,
        All
    };

    HitObject(int x, int y, int time, HitSound hitSound, std::string_view hitSample, Type type)
        :x{x}, y{y}, time{time}, hitSound{hitSound}, hitSample{hitSample}, type{type}
    {
    }

    HitObject(int x, int y, int time, HitSound hitSound, HitSample hitSample, Type type)
        :x{ x }, y{ y }, time{ time }, hitSound{ hitSound }, hitSample{ hitSample }, type{ type }
    {
    }

    /**
     * @details Hit object syntax :
     *      x, y, time, type, hitSound, objectParams, hitSample
     */
    HitObject(std::array<std::string_view, 7> const& result)
        : HitObject{
            std::stoi(result[0].data()),
            std::stoi(result[1].data()),
            std::stoi(result[2].data()),
            static_cast<HitSound>(std::stoi(result[4].data())),
            result[6],
            GetType(result[3])
        }
    {
        isNewCombo = std::stoi(result[3].data()) & ComboBit;
    }

    HitObject(std::string_view line) : HitObject(details::SplitString<7>(line))
    {
    }

    
    /**
     * @brief Calculate the distance to another hit object
     * @param anotherObject the other hit object
     */
    [[nodiscard]] auto distanceTo(HitObject const& anotherObject) const
    {
        return sqrt(pow(x - anotherObject.x, 2) + pow(y - anotherObject.y, 2));
    }

    /**
     * @brief Calculate the time duration to another hit object
     * @param 
     */
    [[nodiscard]] auto timeTo(HitObject const& anotherObject) const
    {
        return std::abs(time - anotherObject.time);
    }

    /**
     * @brief Returns the columnIndex (starts from 0) in mania mode
     * @param columnCount number of total columns
     */
    [[nodiscard]] auto getColumnIndex(int columnCount) const
    {
        return std::clamp(x * columnCount / 512, 0, columnCount - 1);
    }

    /**
     * @brief Calculate the X coordinate in terms of columnIndex and total columns
     */
    [[nodiscard]] static int ColumnToX(int columnIndex, int columnCount)
    {
        /*
         *   columnIndex = x * columnCount / 512
         *   x = columnIndex * 512 / columnCount;
        */
        assert(!(columnIndex < 0 || columnIndex >= columnCount));
        return std::clamp(columnIndex * 512 / (columnCount % 2 == 0? columnCount : columnCount - 1), 0, 512);
    }

    /**
     * @brief Parse hit objects
     * @param file The `.osu` file stream
     * @param partial Whether this is a partial parse, when true, it keeps reading the file until "[HitObjects]" is found
     * @return `std::vector<std::unique_ptr<HitObject>>`
     */
    static auto HandleHitObjects(std::ifstream& file, bool partial = true)
    {
        std::string line;
        std::vector<std::unique_ptr<HitObject>> objects;

        /*If this is a partial parse, skip until [HitObjects] appears*/
        if (partial)
            details::SkipUntil("[HitObjects]", line, file);

        while (details::GetLine(file, line))
        {
            auto [_, __, ___, typeStr] = details::SplitString<4>(line);
            try
            {
                switch (HitObject::GetType(typeStr))
                {
                    case Type::Circle:
                        objects.emplace_back(std::make_unique<Circle>(line));
                        break;
                    case Type::Slider:
                        objects.emplace_back(std::make_unique<Slider>(line));
                        break;
                    case Type::Spinner:
                        objects.emplace_back(std::make_unique<Spinner>(line));
                        break;
                    case Type::Hold:
                        objects.emplace_back(std::make_unique<Hold>(line));
                        break;
                    default:
                        throw std::runtime_error{ "Hit object type not implemented" };
                }
            }
            catch (...)
            {
                std::cerr << "Parsing Line: " << line << "failed!\n";
            }
        }

        /*
            I found an old map (osu file format v11) that doesn't set the first object's comboBit while it should be a new combo
            eg. Title:Three Magic
                TitleUnicode:Three Magic
                Artist:3L
                ArtistUnicode:3L
                Creator:cRyo[iceeicee]
                Version:Collab
            That has the first object like this:
                168,256,33011,2,0,B|120:272|80:240,1,90,4|0,0:0|0:0,0:0:0
                              ^
        */
        if (!objects.empty())
            objects.front()->isNewCombo = true;
        
        return objects;
    }

    /**
     * @brief 
     */
    virtual void printObjectParam(std::ostream& os) const = 0;

    virtual std::unique_ptr<HitObject> clone() const = 0;
    
    /* 
        @details Hit object syntax :
        x, y, time, type, hitSound, objectParams, hitSample
         168, 326, 620, 5, 0, 1 : 0 : 0 : 0 
    */
    friend std::ostream& operator<<(std::ostream& os, HitObject const& hitObject)
    {
        os << hitObject.x << ','
            << hitObject.y << ','
            << hitObject.time << ','
            << (hitObject.isNewCombo ? (static_cast<int>(hitObject.type) | HitObject::ComboBit) : static_cast<int>(hitObject.type)) << ',';
        
        os << static_cast<int>(hitObject.hitSound) << ',';

        /*print object params*/
        hitObject.printObjectParam(os);

        os << hitObject.hitSample;
        return os;
    }

    virtual ~HitObject() = default;

    Type type;
protected:
    static inline constexpr Type GetType(int num)
    {
        if (num & CircleBit)        return Type::Circle;
        else if (num & SliderBit)   return Type::Slider;
        else if (num & SpinnerBit)  return Type::Spinner;
        else if (num & HoldBit)     return Type::Hold;
        else throw std::logic_error{ std::string{"Invalid hit object type: "} + std::to_string(num) };
    }

    static inline Type GetType(std::string_view str)
    {
        return GetType(std::stoi(str.data()));
    }

    static inline bool IsNewCombo(int num) { return num & ComboBit; }

    static inline bool IsNewCombo(std::string_view str) { return IsNewCombo(std::stoi(str.data())); }

    friend struct OsuFile;

private:
    template<Type type> struct ToType;

    template<Type type> using ToType_t = typename ToType<type>::type;
};
template<> struct HitObject::ToType<HitObject::Type::Circle> { using type = Circle; };
template<> struct HitObject::ToType<HitObject::Type::Slider> { using type = Slider; };
template<> struct HitObject::ToType<HitObject::Type::Spinner> { using type = Spinner; };
template<> struct HitObject::ToType<HitObject::Type::Hold> { using type = Hold; };
template<> struct HitObject::ToType<HitObject::Type::All> { using type = HitObject; };

inline std::ostream& operator<<(std::ostream& os, HitObject::Type type)
{
    switch (type)
    {
        case HitObject::Type::Circle:
            os << "Circle";
            break;
        case HitObject::Type::Slider:
            os << "Slider";
            break;
        case HitObject::Type::Spinner:
            os << "Spinner";
            break;
        case HitObject::Type::Hold:
            os << "Hold";
            break;
        default:
            break;
    }
    return os;
}

struct Circle final: HitObject
{
    Circle(std::string_view line) : Circle(details::SplitString<6>(line))
    {
    }

    Circle(int x, int y, int time, HitSound hitSound, HitSample hitSample)
        : HitObject{x, y, time, hitSound, hitSample, Type::Circle}
    {}

    Circle(std::array<std::string_view, 6> const& result) 
        : HitObject{
            std::array<std::string_view, 7>{
                result[0],      //x
                result[1],  //y
                result[2],  //time
                result[3],  //type
                result[4],  //hitsound,
                "",         //object param
                result[5]
            } 
        }
    {}

    void printObjectParam(std::ostream& os) const override
    {
    }

    std::unique_ptr<HitObject> clone() const override
    {
        return std::unique_ptr<HitObject>(new Circle(*this));
    }

    static auto MakeManiaHitObject(int columnIndex, int totalColumns, int time, HitSound hitSound = HitSound::Normal, HitSample hitSample = HitSample{})
    {
        return std::make_unique<Circle>(HitObject::ColumnToX(columnIndex, totalColumns), 0, time, hitSound, std::move(hitSample));
    }
};

struct EdgeSet
{
    int normalSet;
    int additionSet;
    EdgeSet(std::string_view str)
    {
        auto [normalSetStr, additionSetStr] = details::SplitKeyVal(str);
        normalSet = std::stoi(normalSetStr.data());
        additionSet = std::stoi(additionSetStr.data());
    }

    friend std::ostream& operator<<(std::ostream& os, EdgeSet edgeSet)
    {
        os << edgeSet.normalSet << ':' << edgeSet.additionSet;
        return os;
    }
};

/**
 * @details Slider syntax:
 *      x,y,time,type,hitSound,curveType|curvePoints,slides,length,edgeSounds,edgeSets,hitSample
 */
struct Slider final: HitObject
{
    enum class CurveType
    {
        Bezier='B',
        CatmullRom='C',
        Linear='L',
        Circle='C'
    };



    CurveType curveType;
    std::vector<Coord> curvePoints;
    int slides;
    float length;
    std::vector<int> edgeSounds;
    std::vector<EdgeSet> edgeSets;

    Slider(std::string_view line) : Slider(details::SplitString<11>(line))
    {
    }

    Slider(std::array<std::string_view, 11> const& result)
        : HitObject(std::array<std::string_view, 7>
            {
                result[0],  //x
                result[1],  //y
                result[2],  //time
                result[3],  //type
                result[4],  //hitSound
                "",         //objectParams(unused)
                result[10]
            }
        )
    {
        auto const& sliderParam = result[5];
        
        auto [curveTypeChar, curvePointsStr] = details::SplitKeyVal(sliderParam, '|');
        curveType = static_cast<CurveType>(curveTypeChar.front());
        for (auto const& curvePointStr : details::SplitString(curvePointsStr, '|'))
        {
            auto [x, y] = details::SplitKeyVal(curvePointStr);
            curvePoints.push_back(Coord{ std::stoi(x.data()), std::stoi(y.data()) });
        }

        slides = std::stoi(result[6].data());
        length = std::stof(result[7].data());

        if (!result[8].empty())
        {
            for (auto const& edgeSoundStr : details::SplitString(result[8], '|'))
                edgeSounds.push_back(std::stoi(edgeSoundStr.data()));
        }

        if (!result[9].empty())
        {
            for (auto const& edgeSetStr : details::SplitString(result[9], '|'))
                edgeSets.push_back(edgeSetStr);
        }
    }

    void printObjectParam(std::ostream& os) const override
    {
        details::PrintHelper helper{ os };
        helper.print(static_cast<char>(curveType))
            .print('|')
            .print(curvePoints, "|")
            .print(',')
            .print(slides)
            .print(',')
            .print(length)
            .print(',')
            .print(edgeSounds, "|")
            .print(',')
            .print(edgeSets, "|")
            .print(',');
    }

private:
    /**
     * @brief Throw `std::logic_error` if a timing point is inherited.
     */
    static void ThrowIfInherited(TimingPoint const& timingPoint)
    {
        if (timingPoint.uninherited == false || timingPoint.beatLength < 0)
            throw std::logic_error{ "The timing point is inherited" };
    }
public:
    /**
     * @brief Get the duration of this slider
     * @param sliderMultiplier Should be parsed from `Difficulty::sliderMultipler`
     * @param beatLength Should be a beat length accounting for the slider velocity percent in inherited timing point
     * @return Duration in milliseconds
     */
    [[nodiscard]] auto getDuration(float sliderMultiplier, float beatLength) const
    {
        return static_cast<double>(length) * beatLength * slides * 0.01 / sliderMultiplier;
    }

    /**
     * @brief Get the duration of this slider
     * @param difficulty Parsed difficulty section
     * @param uninheritedTimingPoint Must be an uninheirted timingPoint
     * @return Duration in milliseconds
     */
    [[nodiscard]] auto getDuration(Difficulty const& difficulty, TimingPoint const& uninheritedTimingPoint)
    {
        ThrowIfInherited(uninheritedTimingPoint);
        return getDuration(difficulty.sliderMultiplier, uninheritedTimingPoint.beatLength);
    }

    /**
     * @brief Get the duration of this slider
     * @param difficulty Parsed difficulty section
     * @param uninheritedTimingPoint Must be an uninheirted timingPoint
     * @param inheritedTimingPointBeatLength The beatLength value from an inheritedTimingPoint, which is the original negative value
     * @return Duration in milliseconds
     */
    [[nodiscard]] auto getDuration(Difficulty const& difficulty, TimingPoint const& uninheritedTimingPoint, float inheritedTimingPointBeatLength) const
    {
        if (uninheritedTimingPoint.uninherited == false || uninheritedTimingPoint.beatLength < 0 || inheritedTimingPointBeatLength >= 0)
            throw std::logic_error{ "The timing point is inherited" };
        return getDuration(difficulty.sliderMultiplier, uninheritedTimingPoint.beatLength * (-inheritedTimingPointBeatLength) / 100.f);
    }

    std::unique_ptr<HitObject> clone() const override
    {
        return std::unique_ptr<Slider>(new Slider(*this));
    }
};


struct Spinner final : HitObject
{
    int endTime;
       
    Spinner(std::string_view line) : Spinner(details::SplitString<7>(line))
    {
    }

    /**
     * @details Spinner syntax:
     *      x,y,time,type,hitSound,endTime,hitSample
     */
    Spinner(std::array<std::string_view, 7> const& result)
        : HitObject{ std::array<std::string_view, 7>{
            PlayField::xMid,    //x
            PlayField::yMid,    //y
            result[2],          //time
            result[3],          //type
            result[4],          //hitsound
            "",                 //object param(unused)F
            result[6]           //hitSample
        }}
    {
        endTime = std::stoi(result[5].data());
    }

    void printObjectParam(std::ostream& os) const override
    {
        os << endTime << ',';
    }

    std::unique_ptr<HitObject> clone() const override
    {
        return std::unique_ptr<HitObject>(new Spinner(*this));
    }
};

struct Hold final : HitObject
{
    int endTime;

    Hold(std::string_view line) : Hold(details::SplitString<6>(line))
    {
    }

    /**
     * @details Hold syntax:
     *      x,y,time,type,hitSound,endTime:hitSample
     *  @details Hit object syntax:
     *      x,y,time,type,hitSound,objectParams,hitSample
     */

    Hold(int x, int y, int time, HitSound hitSound, int endTime, std::string_view hitSample)
        : HitObject{x, y, time, hitSound, hitSample, Type::Hold }, endTime{endTime}
    {}

    Hold(int x, int y, int time, HitSound hitSound, int endTime, HitSample hitSample)
        : HitObject{x, y, time, hitSound, std::move(hitSample), Type::Hold}, endTime{endTime}
    {}


    Hold(std::array<std::string_view, 6> const& result)
        : Hold{
            std::stoi(result[0].data()),
            std::stoi(result[1].data()),
            std::stoi(result[2].data()),
            static_cast<HitSound>(std::stoi(result[3].data())),
            {},
            ""
        }
    {
        auto [endTimeStr, hitSampleStr] = details::SplitKeyVal(result.back());
        endTime = std::stoi(endTimeStr.data());
        hitSample = HitSample{ hitSampleStr };
    }

    [[nodiscard]] auto getDuration() const
    {
        return endTime - time;
    }

    static auto MakeManiaHitObject(int columnIndex, int totalColumns, int time, int endTime, HitSound hitSound = HitSound::Normal, HitSample hitSample = HitSample{})
    {
        return std::make_unique<Hold>(HitObject::ColumnToX(columnIndex, totalColumns), 0, time, hitSound, endTime, std::move(hitSample));
    }


    void printObjectParam(std::ostream& os) const override
    {
        os << endTime << ':';
    }

    std::unique_ptr<HitObject> clone() const override
    {
        return std::unique_ptr<HitObject>(new Hold(*this));
    }
};

struct Colors
{
    struct Color
    {
        unsigned char r, g, b;

        Color(std::string_view colorString)
        {
            auto const result = details::SplitString<3>(colorString);

            r = std::stoi(result[0].data());
            g = std::stoi(result[1].data());
            b = std::stoi(result[2].data());
        }

        Color(unsigned char r, unsigned char g, unsigned char b) : r{ r }, g{ g }, b{ b }{}

        bool operator==(Color const& rhs) const
        {
            return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
        }

        friend std::ostream& operator<<(std::ostream& os, Color const& color)
        {
            os << static_cast<int>(color.r) << ',' 
                << static_cast<int>(color.g) << ',' 
                << static_cast<int>(color.b);
            return os;
        }
    };

    /**
     * @brief Additive combo colours
     */
    std::vector<Color> comboColor;

    /**
     * @brief Additive slider track colour
     */
    std::optional<Color> sliderTrackOverride;

    /**
     * @brief Slider border color
     */
    std::optional<Color> sliderBorder;

    Colors(std::ifstream& file, bool partial = true)
    {
        std::string line;

        if (partial)
            details::SkipUntil("[Colours]", line, file);

        while (details::GetLine(file, line))
        {
            auto [key, value] = details::SplitKeyVal(line);

            if (key.find("Combo") != std::string::npos) comboColor.emplace_back(std::move(value));
            else if (key == "SliderTrackOverride") sliderTrackOverride = Color{ value };
            else if (key == "SliderBorder") sliderBorder = Color{ value };
        }
    }

    Colors() = default;

    friend std::ostream& operator<<(std::ostream& os, Colors const& colors)
    {
        os << "[Colours]\n";
        for (size_t i = 0; i < colors.comboColor.size(); ++i)
            os << "Combo" << i << " : " << colors.comboColor[i] << '\n';
        if (colors.sliderTrackOverride)
            os << "SliderTrackOverride : " << *colors.sliderTrackOverride << '\n';
        if (colors.sliderBorder)
            os << "SliderBorder : " << *colors.sliderBorder << '\n';
        return os;
    }
};




/**
 * @brief Information used to identify the originalBeatmap
 * @see https://github.com/ppy/osu-wiki/blob/master/wiki/osu!_File_Formats/Osu_(file_format)/en.md#metadata
 */
struct Metadata
{
    /**
     * @brief Difficulty ID
     * @note An osu originalBeatmap relates to 1 difficulty, therefore it's actually 1 difficulty of a certain originalBeatmap set.
     * If not present, the value is -1
     */
    int beatmapId = -1;

    /**
     * @brief Beatmap ID
     * @note If not present, the value is -1
     */
    int beatmapSetId = -1;

    /**
     * @brief Romanised song title
     */
    std::string title;

    /**
     * @brief Song title
     */
    std::string titleUnicode;

    /**
     * @brief Romanised song artist
     */
    std::string artist;

    /**
     * @brief Song artist
     */
    std::string artistUnicode;

    /**
     * @brief Beatmap creator
     */
    std::string creator;

    /**
     * @brief Difficulty name
     */
    std::string version;

    /**
     * @brief Original media the song was produced for
     */
    std::string source;

    /**
     * @brief Search terms
     */
    std::vector<std::string> tags;

    /**
     * @brief Parse Metadata from osu file
     * @param partial If this is a partial parse, it skips until "[Metadata]" line is encountered, default = `true`
     */
    Metadata(std::ifstream& osuFile, bool partial = true)
    {
        std::string line;

        if (partial)
            details::SkipUntil("[Metadata]", line, osuFile);

        while (details::GetLine(osuFile, line))
        {
            auto [key, value] = details::SplitKeyVal(line);

            if (key == "Title")                 title = value;
            else if (key == "TitleUnicode")     titleUnicode = value;
            else if (key == "Artist")           artist = value;
            else if (key == "ArtistUnicode")    artistUnicode = value;
            else if (key == "Creator")          creator = value;
            else if (key == "Version")          version = value;
            else if (key == "Source")           source = value;
            else if (key == "Tags")             tags = details::SplitWords(value);
            else if (key == "BeatmapID")        beatmapId = std::stoi(value.data());
            else if (key == "BeatmapSetID")     beatmapSetId = std::stoi(value.data());
        }
    }

    /**
     * @brief Default constructor, all members set to empty.
     */
    Metadata() = default;

    friend std::ostream& operator<<(std::ostream& os, Metadata const& data)
    {
        details::PrintHelper h{ os };
            h.printLn("[Metadata]")
            .printLn("Title: ", data.title)
            .printLn("TitleUnicode: ", data.titleUnicode)
            .printLn("Artist: ", data.artist)
            .printLn("ArtistUnicode: ", data.artistUnicode)
            .printLn("Creator: ", data.creator)
            .printLn("Version: ", data.version)
            .printLn("Source: ", data.source);
        if (data.beatmapId != -1)
            h.printLn("BeatmapID", data.beatmapId);
        if (data.beatmapSetId != -1)
            h.printLn("BeatmapSetID", -1);

        h.printLn("Tags: ", data.tags, " ");
        return os;
    }
};

struct Background;
struct Video;
struct Break;

struct EventBase
{
    /**
     * @brief Type of the event.
     * @details Some events may be referred to by either a name or a number.
     */
    std::variant<int, std::string> eventType;

    /**
     * @brief Start time of the event, in milliseconds from the beginning of the originalBeatmap's audio.
     */
    int startTime;

    EventBase(std::string_view eventTypeStr, int startTime) : startTime{ startTime }
    {
        try
        {
            eventType = std::stoi(eventTypeStr.data());
        }
        catch (const std::invalid_argument&)
        {
            eventType = std::string{ eventTypeStr };
        }
    }

    EventBase(std::string_view eventTypeStr) : EventBase(eventTypeStr, 0) {}

    EventBase(int eventType, int startTime) : eventType{ eventType }, startTime{ startTime }{}

    virtual ~EventBase() = default;

    bool operator<(EventBase const& other) const
    {
        return startTime < other.startTime;
    }
};

struct Background final : EventBase
{
    /**
     * @brief Location of the background image relative to the originalBeatmap directory.
     * @details Double quotes are usually included surrounding the filename, but they are not required.
     */
    std::string fileName;

    /**
     * @brief Offset in osu! pixels from the center of the screen.
     * @details For example, an offset of 50,100 would have the background shown 50 osu! pixels to the right and 100 osu! pixels down from the center of the screen.
     * @note If the offset is 0,0, writing it is optional.
     */
    int xOffset;

    /**
     * @brief Offset in osu! pixels from the center of the screen.
     * @details For example, an offset of 50,100 would have the background shown 50 osu! pixels to the right and 100 osu! pixels down from the center of the screen.
     * @note If the offset is 0,0, writing it is optional.
     */
    int yOffset;

    Background(std::string_view fileName, int xOffset, int yOffset)
        : EventBase{ 0, 0 }, xOffset{ xOffset }, yOffset{ yOffset }, fileName{ details::TrimStr(fileName, "\"")}
    {}
private:
    Background(std::array<std::string_view, 5> const& data)
        : Background(data[2], data[3].empty()? 0:  std::stoi(data[3].data()), data[4].empty()?  0: std::stoi(data[4].data()))
    {}
public:
    Background(std::string_view line)
        :Background(details::SplitString<5>(line, ','))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, Background const& data)
    {
        os << "0,0,"<< data.fileName << "," << data.xOffset << "," << data.yOffset;
        return os;
    }
};

struct Video final : EventBase
{
    /**
     * @brief Location of the background image relative to the originalBeatmap directory.
     * @details Double quotes are usually included surrounding the filename, but they are not required.
     */
    std::string fileName;

    /**
     * @brief Offset in osu! pixels from the center of the screen.
     * @details For example, an offset of 50,100 would have the background shown 50 osu! pixels to the right and 100 osu! pixels down from the center of the screen.
     * @note If the offset is 0,0, writing it is optional.
     */
    int xOffset;

    /**
     * @brief Offset in osu! pixels from the center of the screen.
     * @details For example, an offset of 50,100 would have the background shown 50 osu! pixels to the right and 100 osu! pixels down from the center of the screen.
     * @note If the offset is 0,0, writing it is optional.
     */
    int yOffset;

    Video(std::string_view video, std::string_view startTime, std::string_view fileName, int xOffset, int yOffset)
        : EventBase{ video, std::stoi(startTime.data()) }, xOffset{ xOffset }, yOffset{ yOffset }
    {}
private:
    Video(std::array<std::string_view, 5> const& data)
        : Video(data[0], data[1], data[2], std::stoi(data[3].data()), std::stoi(data[4].data()))
    {}
public:
    Video(std::string_view line)
        :Video(details::SplitString<5>(line, ','))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, Video const& data)
    {
        os << "1,"<< data.startTime << "," << data.fileName << "," << data.xOffset << "," << data.yOffset;
        return os;
    }
};

struct Break final : EventBase
{
    /**
     * @brief End time of the break, in milliseconds from the beginning of the originalBeatmap's audio.
     */
    int endTime;

    Break(int startTime, int endTime)
        : EventBase{ 2, startTime }, endTime{ endTime }
    {}
private:
    Break(std::array<std::string_view, 3> const& data)
        : Break(std::stoi(data[1].data()), std::stoi(data[2].data()))
    {}
public:
    Break(std::string_view line)
        : Break(details::SplitString<3>(line, ','))
    {}

    [[nodiscard]] auto getDuration() const
    {
        return endTime - startTime;
    }

    friend std::ostream& operator<<(std::ostream& os, Break const& data)
    {
        os << "2," << data.startTime << "," << data.endTime;
        return os;
    }
};

struct Events
{

    enum class Type
    {
        Background = 0,
        Video = 1,
        Break = 2,
    };

    std::vector<Background> backgrounds;
    std::vector<Video> videos;
    std::vector<Break> breaks;

    Events(std::ifstream& file, bool partial = true)
    {
        std::string line;

        if (partial)
            details::SkipUntil("[Events]", line, file);

        while (details::GetLine(file, line))
        {
            auto [eventType, _, __] = details::SplitString<3>(line);
            try {
                auto const eventId = static_cast<Type>(std::stoi(eventType.data()));
                switch (eventId)
                {
                    case Type::Background:
                        backgrounds.emplace_back(line);
                        break;
                    case Type::Video:
                        videos.emplace_back(line);
                        break;
                    case Type::Break:
                        breaks.emplace_back(line);
                        break;
                    default:
                        continue;
                }
            }
            catch (...)
            {
                continue;
            }
        }
    }

    Events() = default;

    Events& operator+=(Background backgroundEvent) { backgrounds.emplace_back(std::move(backgroundEvent)); return *this; }
    Events& operator+=(Video videoEvent) { videos.emplace_back(std::move(videoEvent)); return *this; }
    Events& operator+=(Break breakEvent) { breaks.emplace_back(std::move(breakEvent)); return *this; }
    
    /**
     * @brief Get an event at a specified time
     * @param time Time in milliseconds
     * @tparam eventType Must be an enum from `Events::Type`
     * @return An iterator pointing to the specified type of event that happens before the specified time
     */
    template<Type eventType>// typename = std::enable_if_t<std::is_base_of_v<Evc>
    auto getEventAt(int time)
    {
        if constexpr(eventType == Type::Background)
            return std::find_if(begin(backgrounds), end(backgrounds), [time](auto const& event) { return event.startTime <= time; });
        else if constexpr(eventType == Type::Video)
            return std::find_if(begin(videos), end(videos), [time](auto const& event) { return event.startTime <= time; });
        else if constexpr(eventType == Type::Break)
            return std::find_if(begin(breaks), end(breaks), [time](auto const& event) { return event.startTime <= time; });
    }

    friend std::ostream& operator<<(std::ostream& os, Events const& events)
    {
        os << "[Events]\n";
        auto PrintEventVec = [&os](auto const& vec) { std::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<decltype(vec.front())> {os, "\n"}); };
        
        PrintEventVec(events.backgrounds);
        PrintEventVec(events.breaks);
        PrintEventVec(events.videos);
        return os;
    }
};


struct OsuFile
{
    General general;
    Editor editor;
    Metadata metaData;
    Difficulty difficulty;
    Events events;
    std::vector<TimingPoint> timingPoints;
    Colors colors;
    std::vector<std::unique_ptr<HitObject>> hitObjects;

    OsuFile(std::ifstream&& file)
    {
        std::string line;
        line.reserve(100);
        while (details::GetLine(file, line, false))
        {
            if (line == "[General]")            general = General{ file, false };
            else if (line == "[Editor]")        editor = Editor{ file, false };
            else if (line == "[Metadata]")      metaData = Metadata{ file, false };
            else if (line == "[Difficulty]")    difficulty = Difficulty{ file, false };
            else if (line == "[Events]")        events = Events(file, false);
            else if (line == "[TimingPoints]")  timingPoints = TimingPoint::HandleTimingPoints(file, false);
            else if (line == "[Colours]")       colors = Colors{ file, false };
            else if (line == "[HitObjects]")    hitObjects = HitObject::HandleHitObjects(file, false);
        }
    }

    OsuFile(General const& general, Editor const& editor, Metadata const& metaData, Difficulty const& difficulty, Events const& events, std::vector<TimingPoint> const& timingPoints = {}, Colors const& colors = {})
        : general{ general }, editor{ editor }, metaData{ metaData }, difficulty{ difficulty }, events{ events }, timingPoints{ timingPoints }, colors{ colors }
    {
    }

    OsuFile(General const& general, Editor const& editor, Metadata const& metaData, Difficulty const& difficulty, Events const& events, std::vector<TimingPoint> const& timingPoints, Colors const& colors, std::vector<std::unique_ptr<HitObject>> hitObjects)
        : general{ general }, editor{ editor }, metaData{ metaData }, difficulty{ difficulty }, events{ events }, timingPoints{ timingPoints }, colors{ colors }, hitObjects{ std::move(hitObjects) }
    {
    }

    /**
     * @brief Construct a new originalBeatmap with the original originalBeatmap's information and no hit objects
     */
    OsuFile(OsuFile const& original)
        : OsuFile{ original.general, original.editor, original.metaData, original.difficulty, original.events, original.timingPoints, original.colors }
    {
    }

    OsuFile(OsuFile&& temp) = default;


    OsuFile() = default;
    
    auto& operator[](size_t index)
    {
        return hitObjects[index];
    }

    auto const& operator[](size_t index) const
    {
        return hitObjects[index];
    }

    /**
     * @brief Get the number of all hit objects
     */
    [[nodiscard]] auto getCount() const { return hitObjects.size(); }

    /**
     * @brief Get the number of a certain type of hit objects
     * @tparam type Hit object type
     */
    template<HitObject::Type type>
    [[nodiscard]] auto getCount() const
    {
        if constexpr (type == HitObject::Type::All)
            return getCount();
        else
            return std::count_if(hitObjects.cbegin(), hitObjects.cend(), [](auto const& object) {return object->type == type; });
    }

    /**
     * @brief Get the bpm from file
     * @details
     *    The bpm is stored in the first line in the [TimingPoints] section, as beatLength in milliseconds
     *    As a result, to get BPM, use 60000 (milliseconds per minute) / this number
     */
    [[nodiscard]] auto getBPM() const
    {
        return timingPoints.empty() ? 0.0 : 60'000 / timingPoints.front().beatLength;
    }

    [[nodiscard]] auto getAverageBeatLength() const
    {
        int uninheritedTimingPointCount{};
        return std::accumulate(
                    timingPoints.cbegin(),
                    timingPoints.cend(),
                    0.0,
                    [&uninheritedTimingPointCount](auto sum, auto const& timingPoint) 
                    {
                        if(timingPoint.uninherited)
                            return sum;
                        else
                        {
                            uninheritedTimingPointCount++;
                            return sum + timingPoint.beatLength;
                        }
                    }
                ) 
        / uninheritedTimingPointCount;;
    }

    [[nodiscard]] auto getAverageBPM() const
    {
        return 60'000 / getAverageBeatLength();
    }

    /**
     * @brief Get the nearest timing point to `time`, that is the one nearest timing point before time
     */
    [[nodiscard]] TimingPoint const& getNearestTimingPointAt(int time) const
    {
        if (auto it = std::lower_bound(timingPoints.cbegin(), timingPoints.cend(), time, [](TimingPoint const& timingPoint, int time)
        {
            return timingPoint.time < time;
        }); it != timingPoints.begin())
            return *(it - 1);
        else
            return *timingPoints.begin();
    }

    /**
     * @brief Get timing point that actually controls `time`. If none, returns the first timing point
     */
    template<TimingPoint::Type type = TimingPoint::Type::All>
    [[nodiscard]] TimingPoint const& getTimingPointAt(int time) const
    {
        if (timingPoints.empty())
            throw std::logic_error{ "No timing points!" };

        if (auto const it = std::find_if(timingPoints.cbegin(), timingPoints.cend(), [time](TimingPoint const& point)
        {
            return point.time == time && point.isType<type>();
        }); it != timingPoints.cend())
            return *it;
        else
            return *timingPoints.cbegin();
    }

    /**
     * @brief Calculate total break time (in milliseconds) of a map
     */
    [[nodiscard]] int getTotalBreakTime() const
    {
        return std::accumulate(
            events.breaks.cbegin(), 
            events.breaks.cend(), 
            0, 
            [](auto value, auto const& breakEvent)
            {
                return value + (breakEvent.endTime - breakEvent.startTime);
            }
        );
    }


    /**
     * @brief Get percent of a certain type of hit objects
     * @tparam types One of hit object type in `HitObject::Type` enumeration
     */
    template<HitObject::Type... types>
    [[nodiscard]] float getPercentOf() const
    {
        if constexpr (sizeof...(types) == 0)
            return 0;
        else
            return (getCount<types>() + ...) / static_cast<float>(hitObjects.size());
    }

    [[nodiscard]] int getNumHitObjectDuring(int startTime, int endTime) const
    {
        auto const iter = std::find_if(hitObjects.cbegin(), hitObjects.cend(), [startTime](auto const& obj)
        {
            return obj->time >= startTime;
        });
        if(iter != hitObjects.cend())
        {
            auto const iter2 = std::find_if(iter, hitObjects.cend(), [endTime](auto const& obj) { return obj->time >= endTime; });
            return std::distance(iter, iter2);
        }

        return 0;
    }

    /**
     * @brief Get percent of hit object of a certain column in Mania Mode
     */
    [[nodiscard]] float getPercentOfHitObjectInColumn(int columnIndex) const
    {
        if (general.mode != Mode::Mania || columnIndex < 0 || columnIndex >= difficulty.circleSize)
            throw std::logic_error{ "Invalid call to getPercentOfHitObjectInColumn()" };

        return std::count_if(
            hitObjects.cbegin(),
            hitObjects.cend(),
            [columnIndex, totalColumn = difficulty.circleSize](auto const& obj)
            {
                return obj->getColumnIndex(totalColumn) == columnIndex;
            }
        ) / static_cast<float>(hitObjects.size());
    }

    /**
     * @brief Serialize everything to the file stream
     * @throw std::runtime_error if the file cannot open
     */
    void save(std::ofstream&& file) const
    {
        if (!file.is_open())
            throw std::runtime_error{ "File not writable!" };
        details::PrintHelper{ file }
            .printLn("osu file format v14")
            .printLn("")
            .printLn(general)
            .printLn(editor)
            .printLn(metaData)
            .printLn(difficulty)
            .printLn("[TimingPoints]")
            .printLn(timingPoints)
            .printLn("")
            .printLn(colors)
            .printLn("")
            .printLn(events)
            .printLn("")
            .printLn("[HitObjects]")
            .printLn(hitObjects);
    }

    /**
     * @brief Serialize everything to a `<fileName>.osu`
     * @throw std::runtime_error if the file cannot open
     */
    void save(char const* fileName) const
    {
        constexpr auto suffix = ".osu";
        auto const saveFileName = std::string{ fileName } + suffix;
        std::cout << "Map saved -> " << saveFileName << '\n';
        save(std::ofstream{ saveFileName });
    }

    /**
     * @brief Return the default file name according to song title, artist and difficulty
     * @param withExtension Controls whether `.osu` should be added
     * @details On Windows, some special characters are not allowed in filename, therefore
     * the fields needs to be handled.
     */
    auto getSaveFileName(bool withExtension = false) const
    {
        return details::RemoveInStr(metaData.artist)
            + " - "
            + details::RemoveInStr(metaData.title)
            + " ("
            + details::RemoveInStr(metaData.creator)
            + ") ["
            + details::RemoveInStr(metaData.version)
            + (withExtension ? "].osu" : "]");
    }
public:

    /**
     * @brief Get artist (singer) from an osu file name
     */
    [[nodiscard]] static auto ParseArtistFrom(std::string_view fileName)
    {
        if (auto const pos = fileName.find(" - "); pos != std::string::npos)
            return std::string{ fileName.substr(0, pos) };
        return std::string{};
    }

    /**
     * @brief Get artist (singer) from an osu file path
     */
    [[nodiscard]] static auto ParseArtistFrom(std::filesystem::path const& path)
    {
        return ParseArtistFrom(std::string_view{ path.filename().string() });
    }

    /**
     * @brief Get song title (song name) from an osu file name
     */
    [[nodiscard]] static auto ParseTitleFrom(std::string_view fileName)
    {
        auto const start = fileName.find(" - ");
        auto const end = fileName.find(" (", start);

        if (start != std::string::npos && end != std::string::npos)
        {
            auto const realStart = start + sizeof(" - ") - 1;
            return std::string{ &fileName[realStart], end - realStart };
        }

        return std::string{};
    }

    /**
     * @brief Get song title (song name) from an osu file path
     */
    [[nodiscard]] static auto ParseTitleFrom(std::filesystem::path const& path)
    {
        return ParseTitleFrom(std::string_view{ path.filename().string() });
    }

    /**
     * @brief Get map creator (mapper) from an osu file name
     */
    [[nodiscard]] static auto ParseCreatorFrom(std::string_view fileName)
    {
        auto const start = fileName.find(" (");
        auto const end = fileName.find(") [", start);

        if (start != std::string::npos && end != std::string::npos)
        {
            auto const realStart = start + sizeof(" (") - 1;
            return std::string{ &fileName[realStart], end - realStart };
        }

        return std::string{};
    }

    /**
     * @breif Get map creator (mapper) from an osu file path
     */
    [[nodiscard]] static auto ParseCreatorFrom(std::filesystem::path const& path)
    {
        return ParseCreatorFrom(std::string_view{ path.filename().string() });
    }

    /**
     * @brief Get version (difficulty) from an osu file name
     */
    [[nodiscard]] static auto ParseVersionFrom(std::string_view fileName)
    {
        auto const start = fileName.find(" [");
        auto const end = fileName.find(']', start);

        if (start != std::string::npos && end != std::string::npos)
        {
            auto const realStart = start + sizeof(" [") - 1;
            return std::string{ &fileName[realStart], end - realStart };
        }

        return std::string{};
    }

    /**
     * @brief Get version (difficulty) from an osu file name
     */
    [[nodiscard]] static auto ParseVersionFrom(std::filesystem::path const& path)
    {
        return ParseVersionFrom(std::string_view{ path.filename().string() });
    }

    /**
     * @brief Serialize everything to a `<SongName>[<difficulty>].osu`
     */
    void save() const
    {
        save(getSaveFileName(false).data());
    }

private:

    /**
     * @brief Hit object iterator
     * @tparam ObjectContainer The type of hit object container
     * @tparam type Should be one of the enum values in `HitObject::Type`
     */
    template<typename ObjectContainer, HitObject::Type type, typename HitObjectType>
    class ObjectIterator
    {
    public:
        /*std::iterator<> is deprecated (in C++17) so we define these boilerplate ourselves*/
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename ObjectContainer::iterator::value_type;
        using difference_type = typename ObjectContainer::iterator::difference_type;
        using pointer = value_type*;
        using reference = value_type&;

        ObjectIterator(typename ObjectContainer::iterator iter, typename ObjectContainer::iterator begin, typename ObjectContainer::iterator end) 
            : iter{ iter }, first { std::move(begin) }, last{ std::move(end) }
        {
            if (iter != last)
                advanceIter();
            else
                --(*this);
        }
        auto& operator*() const { return *dynamic_cast<HitObjectType*>(iter->get()); }
        auto* operator->() const { return dynamic_cast<HitObjectType*>(iter->get()); }
        ObjectIterator& operator++()
        {
            advanceIter();
            return *this;
        }
        ObjectIterator& operator--()
        {
            auto const iter_copy = iter;
            while (iter != first && (*iter)->type != type)
                --iter;
            //If iter is already the the first position but the type is not matched, we need to restore it to previous value pointing to the "correct" type of hit object
            if (iter == first && (*iter)->type != type)
                iter = iter_copy;
            return *this;
        }
        bool operator==(ObjectIterator const& rhs) const { return iter == rhs.iter; }
        bool operator!=(ObjectIterator const& rhs) const { return iter != rhs.iter; }
    private:
        typename ObjectContainer::iterator iter;
        typename ObjectContainer::iterator const first;
        typename ObjectContainer::iterator const last;
        
        void advanceIter()
        {
            auto const iter_copy = iter;
            while (iter != last && (*iter)->type != type)
                ++iter;
            //If iter is already at the end position but the type is not matched, we need to restore it to previous value pointing to the "correct" type of hit object
            if (iter == last && (*iter)->type != type)
                iter = iter_copy;
        }
    };

public:

    /**
     * @brief Get the iterator pointing to the first specified type of hit object
     * @tparam type Should be one of the enum values in `HitObject::Type`
     */
    template<HitObject::Type type = HitObject::Type::All>
    auto begin()
    {
        return ObjectIterator<decltype(hitObjects), type, typename HitObject::ToType_t<type>>{hitObjects.begin(), hitObjects.begin(), hitObjects.end()};
    }

    /**
     * @brief Get the iterator pointing to the last specified type of hit object
     * @tparam type Should be one of the enum values in `HitObject::Type`
     */
    template<HitObject::Type type = HitObject::Type::All>
    auto end()
    {
        return ObjectIterator<decltype(hitObjects), type, typename HitObject::ToType_t<type>>{hitObjects.end(), hitObjects.begin(), hitObjects.end()};
    }

    /**
     * @brief Get drain time in milliseconds
     */
    [[nodiscard]] auto getDrainTime() const
    {
        if (auto const count = hitObjects.size(); count == 0 || count == 1)
            return 0;

        if (auto const drainTime = hitObjects.back()->time - hitObjects.front()->time; drainTime > 0)
            return drainTime;

        throw std::logic_error{ "Drain time <= 0" };
    }
};

/**
 * @brief Specialized for generic `ObjectIterator`, that iterate to every hit objects
 */
template<typename ObjectContainer, typename HitObjectType>
class OsuFile::ObjectIterator<ObjectContainer, HitObject::Type::All, HitObjectType>
{
public:
    /*std::iterator<> is deprecated (in C++17) so we define these boilerplate ourselves*/
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename ObjectContainer::iterator::value_type;
    using difference_type = typename ObjectContainer::iterator::difference_type;
    using pointer = value_type*;
    using reference = value_type&;

    ObjectIterator(typename ObjectContainer::iterator iter, typename ObjectContainer::iterator begin, typename ObjectContainer::iterator end) : iter{ iter }, first{ std::move(begin) }, last{ std::move(end) } {}
    HitObject& operator*() const { return *iter->get(); }
    HitObject* operator->() const { return iter->get(); }
    ObjectIterator& operator++()
    {
        //if (iter + 1 != last)
        //    ++iter;
        ++iter;
        return *this;
    }
    ObjectIterator& operator--()
    {
        //if (iter != first)
        //    --iter;
        --iter;
        return *this;
    }
    bool operator==(ObjectIterator const& rhs) const { return iter == rhs.iter; }
    bool operator!=(ObjectIterator const& rhs) const { return iter != rhs.iter; }
private:
    typename ObjectContainer::iterator iter;
    typename ObjectContainer::iterator const first;
    typename ObjectContainer::iterator const last;
};

namespace Utils
{
    /**
     * @brief Get all the entries of beatmaps from a folder
     * @param folder The folder to enumerate
     * @return A vector of `std::filesystem::directory_entry` that have the extension of `.osu`
     */
    inline std::vector<std::filesystem::directory_entry> GetAllBeatmapsFrom(std::filesystem::directory_entry const& folder)
    {
        if (!folder.exists() || !folder.is_directory())
            return {};
        
        std::vector<std::filesystem::directory_entry> result;
        std::copy_if(
            std::filesystem::directory_iterator{ folder.path() },
            std::filesystem::directory_iterator{},
            std::back_inserter(result), 
            [] (auto const& entry)
            {
                return entry.path().extension() == ".osu";
            }
        );

        return result;
    }
}
