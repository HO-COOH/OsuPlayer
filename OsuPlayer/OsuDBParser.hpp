#pragma once
#include <fstream>
#include <vector>
#include <utility>
#include <cassert>
#include <optional>
#include <memory> //for std::unique_ptr
#include <unordered_set>

namespace Db
{
    enum class Mods : unsigned
    {
        None = 0,
        NoFail = 1,
        Easy = 2,
        TouchDevice = 4,
        Hidden = 8,
        HardRock = 16,
        SuddenDeath = 32,
        DoubleTime = 64,
        Relax = 128,
        HalfTime = 256,
        Nightcore = 512, // Only set along with DoubleTime. i.e: NC only gives 576
        Flashlight = 1024,
        Autoplay = 2048,
        SpunOut = 4096,
        Relax2 = 8192,    // Autopilot
        Perfect = 16384, // Only set along with SuddenDeath. i.e: PF only gives 16416  
        Key4 = 32768,
        Key5 = 65536,
        Key6 = 131072,
        Key7 = 262144,
        Key8 = 524288,
        FadeIn = 1048576,
        Random = 2097152,
        Cinema = 4194304,
        Target = 8388608,
        Key9 = 16777216,
        KeyCoop = 33554432,
        Key1 = 67108864,
        Key3 = 134217728,
        Key2 = 268435456,
        ScoreV2 = 536870912,
        Mirror = 1073741824,
        KeyMod = Key1 | Key2 | Key3 | Key4 | Key5 | Key6 | Key7 | Key8 | Key9 | KeyCoop,
        FreeModAllowed = NoFail | Easy | Hidden | HardRock | SuddenDeath | Flashlight | FadeIn | Relax | Relax2 | SpunOut | KeyMod,
        ScoreIncreaseMods = Hidden | HardRock | DoubleTime | Flashlight | FadeIn
    };

    template<typename Underlying>
    class TrivialParsable
    {
        Underlying value{};
    public:
        TrivialParsable() = default;

        TrivialParsable(unsigned char const*& ptr) :
            value(*reinterpret_cast<Underlying const*>(ptr))
        {
            ptr += sizeof(Underlying);
        }


        constexpr operator Underlying() const
        {
            return value;
        }
    };
    using Byte = TrivialParsable<unsigned char>;
    using Short = TrivialParsable<short>;
    using Int = TrivialParsable<int32_t>;
    using Long = TrivialParsable<int64_t>;
    using Single = TrivialParsable<float>;
    using Double = TrivialParsable<double>;
    using Boolean = TrivialParsable<unsigned char>;

    template<typename T>
    std::vector<T> GetArray(unsigned char const*& ptr)
    {
        Int const count{ ptr };
        std::vector<T> result;
        result.reserve(count);
        for (int i = 0; i < count; ++i)
        {
            result.emplace_back(ptr);
        }
        return result;
    }

    template<typename T>
    std::vector<T> GetArray(unsigned char const*& ptr, int count)
    {
        std::vector<T> result;
        result.reserve(count);
        for (int i = 0; i < count; ++i)
        {
            result.emplace_back(ptr);
        }
        return result;
    }

    inline std::unique_ptr<unsigned char[]> LoadBinaryFromStream(std::istream&& fs)
    {
        size_t const bytes = fs.seekg(0, std::ios_base::end).tellg();
        fs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        auto buffer = std::make_unique<unsigned char[]>(bytes);
        fs.seekg(0, std::ios_base::beg).read(reinterpret_cast<char*>(buffer.get()), bytes);

        return buffer;
    }

    inline std::unique_ptr<unsigned char[]> LoadBinaryFromFile(std::ifstream&& fs)
    {
        if (!fs.is_open())
            throw std::runtime_error{ "File not opened" };
        return LoadBinaryFromStream(std::move(fs));
    }

    class ULEB128
    {
        uint64_t value{};
    public:
        ULEB128() = default;
        ULEB128(unsigned char const*& ptr)
        {
            unsigned long result = 0;
            int shift = 0;
            std::size_t count = 0;

            while (true)
            {
                unsigned char byte = *ptr;
                ptr++;
                count++;

                result |= (byte & 0x7f) << shift;
                shift += 7;

                if (!(byte & 0x80)) break;
            }

            value = result;
        }

        operator uint64_t() const
        {
            return value;
        }
    };



    class String : public std::string
    {
    public:
        String() = default;
        String(unsigned char const*& ptr)
        {
            if (*ptr++ != 0x0b)
                return;

            ULEB128 bytes{ ptr };
            (std::string&)(*this) = std::string{ reinterpret_cast<char const*>(ptr), bytes };
            ptr += bytes;
        }
    };



    class IntDoublePair : public std::pair<Int, Double>
    {
    public:
        IntDoublePair() = default;
        IntDoublePair(unsigned char const*& ptr) : std::pair<Int, Double>{}
        {
            if (ptr[0] != 0x08)
                assert(false); //First byte is 0x08
            ++ptr;
            first = *reinterpret_cast<Int const*>(ptr);
            ptr += 4;

            if (ptr[0] != 0x0d)
                assert(false); //The byte is 0x0d
            ++ptr;
            second = *reinterpret_cast<Double const*>(ptr);
            ptr += 8;
        }

    };

    class TimingPoint
    {
        Double bpm;
        Double offset; //in milliseconds
        Boolean uninherited; //false -> inherited
    public:
        TimingPoint() = default;
        TimingPoint(unsigned char const*& ptr)
            : bpm(ptr),
            offset(ptr),
            uninherited(ptr)
        {
        }

    };

    class DateTime
    {
        int64_t ticks{}; //amount of invervals of 100 nanoseconds since midnight Jan1, 0001 UTC
    public:
        DateTime() = default;
        DateTime(unsigned char const*& ptr)
            : ticks{ *reinterpret_cast<int64_t const*>(ptr) }
        {
            ptr += sizeof(int64_t);
        }
    };

    struct Beatmap
    {
        enum class RankStatusEnum : unsigned char
        {
            Unknown = 0,
            Unsubmitted = 1,
            Pending = 2,
            Unused = 3,
            Ranked = 4,
            Approved = 5,
            Qualified = 6,
            Loved = 7
        };

        using RankStatus = TrivialParsable<unsigned char>;

        enum class ModeEnum : unsigned char
        {
            Std = 0x00,
            Taiko = 0x01,
            Ctb = 0x02,
            Mania = 0x03
        };

        using Mode = TrivialParsable<unsigned char>;

        String artistName;
        String artistNameUnicode;
        String songTitle;
        String songTitleUnicode;
        String creator;
        String difficulty;
        String audioFileName;
        String md5;
        String osuFileName;
        RankStatusEnum rankStatus;
        Short numHitCircles;
        Short numSliders;
        Short numSpinners;
        Long lastModified;
        Single approachRate;
        Single circleSize;
        Single hpDrainRate;
        Single overallDifficulty;
        Double sliderVelocity;
        std::vector<IntDoublePair> stdModStarRating;    //mod <=> star rating in std
        std::vector<IntDoublePair> taikoModStarRating;  //mod <=> star rating in taiko
        std::vector<IntDoublePair> ctbModStarRating;    //mod <=> star rating in ctb
        std::vector<IntDoublePair> maniaModStarRating;  //mod <=> star rating in mania
        Int drainTime; //in seconds
        Int totalTime; //in milliseconds
        Int previewTime; //in milliseconds
        std::vector<TimingPoint> timingPoints;
        Int difficultyId;
        Int beatmapId;
        Int threadId;
        Byte stdGrade;
        Byte taikoGrade;
        Byte ctbGrade;
        Byte maniaGrade;
        Short localOffset;
        Single stackLeniency;
        ModeEnum mode;
        String songSource;
        String songTags;
        Short onlineOffset;
        String font;
        Boolean unplayed;
        Long lastPlayed;
        Boolean isOsz2;
        String folderName;
        Long lastChecked; //last time when beatmap was checked against osu! repository
        Boolean ignoreBitmapSound;
        Boolean ignoreSkin;
        Boolean disableStoryboard;
        Boolean disableVideo;
        Boolean visualOverride;
        Int lastModified2;
        Byte maniaScrollSpeed;

        Beatmap(unsigned char const*& ptr) :
            artistName(ptr),
            artistNameUnicode(ptr),
            songTitle(ptr),
            songTitleUnicode(ptr),
            creator(ptr),
            difficulty(ptr),
            audioFileName(ptr),
            md5(ptr),
            osuFileName(ptr),
            rankStatus(static_cast<RankStatusEnum>(static_cast<unsigned char>(RankStatus{ ptr }))),
            numHitCircles(ptr),
            numSliders(ptr),
            numSpinners(ptr),
            lastModified(ptr),
            approachRate(ptr),
            circleSize(ptr),
            hpDrainRate(ptr),
            overallDifficulty(ptr),
            sliderVelocity(ptr),
            stdModStarRating(GetArray<IntDoublePair>(ptr)),
            taikoModStarRating(GetArray<IntDoublePair>(ptr)),
            ctbModStarRating(GetArray<IntDoublePair>(ptr)),
            maniaModStarRating(GetArray<IntDoublePair>(ptr)),
            drainTime(ptr),
            totalTime(ptr),
            previewTime(ptr),
            timingPoints(GetArray<TimingPoint>(ptr)),
            difficultyId(ptr),
            beatmapId(ptr),
            threadId(ptr),
            stdGrade(ptr),
            taikoGrade(ptr),
            ctbGrade(ptr),
            maniaGrade(ptr),
            localOffset(ptr),
            stackLeniency(ptr),
            mode(static_cast<ModeEnum>(static_cast<unsigned char>(Mode{ ptr }))),
            songSource(ptr),
            songTags(ptr),
            onlineOffset(ptr),
            font(ptr),
            unplayed(ptr),
            lastPlayed(ptr),
            isOsz2(ptr),
            folderName(ptr),
            lastChecked(ptr),
            ignoreBitmapSound(ptr),
            ignoreSkin(ptr),
            disableStoryboard(ptr),
            disableVideo(ptr),
            visualOverride(ptr),
            lastModified2(ptr),
            maniaScrollSpeed(ptr)
        {

        }

        Beatmap() = default;

        bool operator==(std::string const& otherMd5) const
        {
            return md5 == otherMd5;
        }
    };

    enum class UserPermission
    {
        None = 0,
        Normal = 1,
        Moderator = 2,
        Supporter = 4,
        Friend = 8,
        Peppy = 16,
        WorldCupStaff = 32
    };
}

namespace std
{
    template<>
    struct hash<Db::Beatmap>
    {
        using is_transparent = std::true_type;
        auto operator()(Db::Beatmap const& beatmap) const
        {
            return std::hash<std::string>{}(beatmap.md5);
        }

        auto operator()(std::string const& md5) const
        {
            return std::hash<std::string>{}(md5);
        }
    };

    template<>
    struct equal_to<Db::Beatmap>
    {
        using is_transparent = std::true_type;
        auto operator()(std::string_view md5, Db::Beatmap const& beatmap) const
        {
            return md5 == beatmap.md5;
        }

        auto operator()(Db::Beatmap const& beatmap, std::string_view md5) const
        {
            return md5 == beatmap.md5;
        }

        auto operator()(Db::Beatmap const& lhs, Db::Beatmap const& rhs) const
        {
            return lhs.md5 == rhs.md5;
        }
    };
}
namespace Db
{


    struct Osu
    {
        Int version{};

        Int folderCount{};

        Boolean accountUnlocked{};

        DateTime unlockAccountDate;

        String playerName;

        Int numBeatmaps{};

        std::vector<Beatmap> beatmaps;

        UserPermission userPermission;

        Osu(unsigned char const* ptr) :
            version(ptr),
            folderCount(ptr),
            accountUnlocked(ptr),
            unlockAccountDate(ptr),
            playerName(ptr),
            numBeatmaps(ptr),
            beatmaps(GetArray<Beatmap>(ptr, numBeatmaps)),
            userPermission(static_cast<UserPermission>(*reinterpret_cast<unsigned char const*>(ptr++)))
        {
        }

        Osu(std::unique_ptr<unsigned char[]>&& ptr) : Osu(ptr.get())
        {
        }

        Osu(std::ifstream&& fs) : Osu(LoadBinaryFromFile(std::move(fs)))
        {
        }

        Osu(std::istream&& fs) : Osu(LoadBinaryFromStream(std::move(fs)))
        {
        }


        auto getBeatmapSet() const
        {
            return std::unordered_set(beatmaps.cbegin(), beatmaps.cend());
        }
    };

    struct Collections
    {
        Int version;

        struct Collection
        {
            String name;
            std::vector<String> md5s;
            Collection(unsigned char const*& ptr) : name(ptr)
            {
                Int const count{ ptr };
                md5s.reserve(count);
                for (int i = 0; i < count; ++i)
                {
                    md5s.emplace_back(ptr);
                }
            }
        };

        std::vector<Collection> collections;

        Collections(unsigned char const* ptr) :
            version{ptr}
        {
            Int const count{ ptr };
            collections.reserve(count);
            for (int i = 0; i < count; ++i)
            {
                collections.emplace_back(ptr);
            }
        }

        Collections(std::unique_ptr<unsigned char[]>&& ptr) : Collections(ptr.get())
        {
        }

        Collections(std::ifstream&& fs) : Collections(LoadBinaryFromFile(std::move(fs)))
        {
        }

        Collections(std::istream&& fs) : Collections(LoadBinaryFromStream(std::move(fs)))
        {
        }
    };

    struct Scores
    {
        Int version;
        
        struct BeatmapScore
        {
            String md5;
            
            struct Score
            {
                Beatmap::ModeEnum mode;
                Int version;
                String beatmapMd5;
                String playerName;
                String replayMd5;
                Short num300;
                Short num100;   //100 in std, 150 in taiko, 100 in ctb, 100 in mania
                Short num50;
                Short numGeki;
                Short numKatus;
                Short numMiss;
                Int replayScore;
                Short maxCombo;
                Boolean perfectCombo;
                Int mods;
            private:
                String _; //always empty
            public:
                Long timestamp;
            private:
                Int __;   //always 0xffffffff
            public:
                Long scoreId;
                std::optional<Double> additionalMod;

                Score(unsigned char const*& ptr) :
                    mode(static_cast<Beatmap::ModeEnum>(static_cast<unsigned char>(Beatmap::Mode{ptr}))),
                    version(ptr),
                    beatmapMd5(ptr),
                    playerName(ptr),
                    replayMd5(ptr),
                    num300(ptr),
                    num100(ptr),
                    num50(ptr),
                    numGeki(ptr),
                    numKatus(ptr),
                    numMiss(ptr),
                    replayScore(ptr),
                    maxCombo(ptr),
                    perfectCombo(ptr),
                    mods(ptr),
                    _(ptr),
                    timestamp(ptr),
                    __(ptr),
                    scoreId(ptr)
                {
                    if (mods & static_cast<unsigned>(Mods::Target))
                    {
                        Double value{ ptr };
                        additionalMod = value;
                    }
                }

            };

            std::vector<Score> scores;

            BeatmapScore(unsigned char const*& ptr) : 
                md5(ptr)
            {
                Int const count{ ptr };
                scores.reserve(count);
                for (int i = 0; i < count; ++i)
                {
                    scores.emplace_back(ptr);
                }
            }
        };

        std::vector<BeatmapScore> beatmapScores;

        Scores(unsigned char const* ptr) :
            version(ptr)
        {
            Int const count{ ptr };
            beatmapScores.reserve(count);
            for (int i = 0; i < count; ++i)
            {
                beatmapScores.emplace_back(ptr);
            }
        }

        Scores(std::unique_ptr<unsigned char[]>&& ptr) : Scores(ptr.get())
        {
        }

        Scores(std::ifstream&& fs) : Scores(LoadBinaryFromFile(std::move(fs)))
        {
        }
    };
};

