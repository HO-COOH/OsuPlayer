//Docs: https://github.com/ppy/osu/wiki/Legacy-database-file-structure#osudb-format
#pragma once
#include <fstream>
#include <vector>
#include <utility>
#include <cassert>
#include <optional>
#include <memory> //for std::unique_ptr
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <numeric>

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
        static constexpr auto bytes()
        {
            return sizeof(Underlying);
        }

        TrivialParsable() = default;

        TrivialParsable(unsigned char const*& ptr) :
            value(*reinterpret_cast<Underlying const*>(ptr))
        {
            ptr += bytes();
        }

        TrivialParsable(Underlying value) : value{ value }
        {
        }

        constexpr operator Underlying() const
        {
            return value;
        }

        void write(unsigned char*& ptr) const
        {
            memcpy(ptr, &value, bytes());
            ptr += bytes();
        }
    };

    using Byte = TrivialParsable<unsigned char>;
    using Short = TrivialParsable<short>;
    using Int = TrivialParsable<int32_t>;
    using Long = TrivialParsable<int64_t>;
    using Single = TrivialParsable<float>;
    using Double = TrivialParsable<double>;
    using Boolean = TrivialParsable<unsigned char>;
    class String;

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

    template<typename T>
    void WriteArrayWithCount(unsigned char*& ptr, std::vector<T> const& data)
    {
        Int{ static_cast<int>(data.size()) }.write(ptr);
        for (auto const& element : data)
            element.write(ptr);
    }

    template<typename T>
    void WriteArrayWithoutCount(unsigned char*& ptr, std::vector<T> const& data)
    {
        for (auto const& element : data)
            element.write(ptr);
    }

    template<typename T>
    auto GetArrayBytesWithoutCount(std::vector<T> const& data)
    {
        return data.front().bytes() * data.size();
    }

    template<typename T>
    auto GetArrayBytesWithCount(std::vector<T> const& data)
    {
        return Int::bytes() + GetArrayBytesWithoutCount(data);
    }




    inline std::unique_ptr<unsigned char[]> LoadBinaryFromStream(std::istream&& fs)
    {
        size_t const bytes = fs.seekg(0, std::ios_base::end).tellg();
        //fs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
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
        uint64_t m_value{};

        constexpr static inline unsigned char LOW_7_BIT_MASK = 0x7f;
        constexpr static inline unsigned char HIGHEST_BIT_MASK = 0x80;

    public:
        ULEB128() = default;

        ULEB128(uint64_t value) : m_value{ value }
        {
        }

        ULEB128(unsigned char const*& ptr)
        {
            unsigned long result = 0;
            int shift = 0;

            while (true)
            {
                unsigned char byte = *ptr;
                ptr++;

                result |= (byte & LOW_7_BIT_MASK) << shift;
                shift += 7;

                if (!(byte & HIGHEST_BIT_MASK)) break;
            }

            m_value = result;
        }

        operator uint64_t() const
        {
            return m_value;
        }

        void write(unsigned char*& ptr) const
        {
            auto valueCopy = m_value;
            do
            {
                unsigned char byte = valueCopy & LOW_7_BIT_MASK; //low order 7 bits
                valueCopy >>= 7;
                if(valueCopy != 0)
                    byte |= HIGHEST_BIT_MASK; //there are more bytes, set the highest bit

                *ptr = byte;
                ++ptr;
            } while (valueCopy != 0);
        }

        auto bytes() const
        {
            size_t byte{};
            auto valueCopy = m_value;
            do 
            {
                valueCopy >>= 7;
                ++byte;
            } while (valueCopy != 0);
            return byte;
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

        void write(unsigned char*& ptr) const
        {
            auto const& stringBase = static_cast<std::string const&>(*this);
            auto const bytes = stringBase.size();
            if (bytes == 0)
            {
                *ptr = 0x00;
                ++ptr;
                return;
            }
            ULEB128{ bytes }.write(ptr);
            if (!stringBase.empty())
                memcpy(ptr, &stringBase.front(), bytes);
            ptr += bytes;
        }

        auto bytes() const
        {
            auto const size = static_cast<std::string const&>(*this).size();
            return size == 0 ? 1 : 1 + ULEB128{ size }.bytes() + size;
        }
    };

    inline auto GetArrayBytesWithoutCount(std::vector<String> const& data)
    {
        return std::accumulate(
            data.cbegin(),
            data.cend(),
            size_t{},
            [](size_t val, String const& curr) { return val + curr.bytes(); }
        );
    }
    inline auto GetArrayBytesWithCount(std::vector<String> const& data)
    {
        return Int::bytes() + GetArrayBytesWithoutCount(data);
    }



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

        void write(unsigned char*& ptr) const
        {
            *ptr = 0x08;    //first byte is 0x08
            ++ptr;
            *reinterpret_cast<Int*>(ptr) = first; //followed by an Int
            ptr += sizeof(Int);
            *ptr = 0x0d;    //this byte is 0x0d
            ++ptr;
            *reinterpret_cast<Double*>(ptr) = second;   //
            ptr += sizeof(Double);
        }

        static constexpr auto bytes()
        {
            return 1 + Int::bytes() + 1 + Double::bytes();
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

        void write(unsigned char*& ptr) const
        {
            bpm.write(ptr);
            offset.write(ptr);
            uninherited.write(ptr);
        }

        static constexpr auto bytes()
        {
            return 2 * Double::bytes() + Boolean::bytes();
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

        void write(unsigned char*& ptr) const
        {
            memcpy(ptr, &ticks, sizeof(ticks));
            ptr += sizeof(ticks);
        }

        static constexpr auto bytes()
        {
            return sizeof(ticks);
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

        void write(unsigned char*& ptr) const
        {
            artistName.write(ptr);
            artistNameUnicode.write(ptr);
            songTitle.write(ptr);
            songTitleUnicode.write(ptr);
            creator.write(ptr);
            difficulty.write(ptr);
            audioFileName.write(ptr);
            md5.write(ptr);
            osuFileName.write(ptr);
            TrivialParsable<unsigned char>{static_cast<unsigned char>(rankStatus)}.write(ptr);
            numHitCircles.write(ptr);
            numSliders.write(ptr);
            numSpinners.write(ptr);
            lastModified.write(ptr);
            approachRate.write(ptr);
            circleSize.write(ptr);
            hpDrainRate.write(ptr);
            overallDifficulty.write(ptr);
            sliderVelocity.write(ptr);
            WriteArrayWithCount(ptr, stdModStarRating);
            WriteArrayWithCount(ptr, taikoModStarRating);
            WriteArrayWithCount(ptr, ctbModStarRating);
            WriteArrayWithCount(ptr, maniaModStarRating);
            drainTime.write(ptr);
            totalTime.write(ptr);
            previewTime.write(ptr);
            WriteArrayWithCount(ptr, timingPoints);
            difficultyId.write(ptr);
            beatmapId.write(ptr);
            threadId.write(ptr);
            stdGrade.write(ptr);
            taikoGrade.write(ptr);
            ctbGrade.write(ptr);
            maniaGrade.write(ptr);
            localOffset.write(ptr);
            stackLeniency.write(ptr);
            TrivialParsable<unsigned char>{static_cast<unsigned char>(mode)}.write(ptr);
            songSource.write(ptr);
            songTags.write(ptr);
            onlineOffset.write(ptr);
            font.write(ptr);
            unplayed.write(ptr);
            lastPlayed.write(ptr);
            isOsz2.write(ptr);
            folderName.write(ptr);
            lastChecked.write(ptr);
            ignoreBitmapSound.write(ptr);
            ignoreSkin.write(ptr);
            disableStoryboard.write(ptr);
            disableVideo.write(ptr);
            visualOverride.write(ptr);
            lastModified2.write(ptr);
            maniaScrollSpeed.write(ptr);
        }

        auto bytes() const
        {
            return
                artistName.bytes() +
                artistNameUnicode.bytes() +
                songTitle.bytes() +
                songTitleUnicode.bytes() +
                creator.bytes() +
                difficulty.bytes() +
                audioFileName.bytes() +
                md5.bytes() +
                osuFileName.bytes() +
                1 +
                numHitCircles.bytes() +
                numSliders.bytes() +
                numSpinners.bytes() +
                lastModified.bytes() +
                approachRate.bytes() +
                circleSize.bytes() +
                hpDrainRate.bytes() +
                overallDifficulty.bytes() +
                sliderVelocity.bytes() +
                GetArrayBytesWithCount(stdModStarRating) +
                GetArrayBytesWithCount(taikoModStarRating) +
                GetArrayBytesWithCount(ctbModStarRating) +
                GetArrayBytesWithCount(maniaModStarRating) +
                drainTime.bytes() +
                totalTime.bytes() +
                previewTime.bytes() +
                GetArrayBytesWithCount(timingPoints) +
                difficultyId.bytes() +
                beatmapId.bytes() +
                threadId.bytes() +
                stdGrade.bytes() +
                taikoGrade.bytes() +
                ctbGrade.bytes() +
                maniaGrade.bytes() +
                localOffset.bytes() +
                stackLeniency.bytes() +
                1 +
                songSource.bytes() +
                songTags.bytes() +
                onlineOffset.bytes() +
                font.bytes() +
                unplayed.bytes() +
                lastPlayed.bytes() +
                isOsz2.bytes() +
                folderName.bytes() +
                lastChecked.bytes() +
                ignoreBitmapSound.bytes() +
                ignoreSkin.bytes() +
                disableStoryboard.bytes() +
                visualOverride.bytes() +
                lastModified2.bytes() +
                maniaScrollSpeed.bytes();
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
    template<typename Derived>
    struct ISerializable
    {
        auto getBuffer() const
        {
            auto& derivedThis = *static_cast<Derived const*>(this);
            auto buffer = std::make_unique<unsigned char[]>(derivedThis.bytes());
            auto rawPtr = buffer.get();
            derivedThis.write(rawPtr);
            return buffer;
        }
    };

    class Osu : public ISerializable<Osu>
    {
        unsigned char const* m_ptrRecord{};
        std::optional<size_t> m_bytes{}; //stored for getting size of buffer for allocation
    public:
        Int version{};

        Int folderCount{};

        Boolean accountUnlocked{};

        DateTime unlockAccountDate;

        String playerName;

        Int numBeatmaps{};

        std::vector<Beatmap> beatmaps;

        UserPermission userPermission;

        Osu(unsigned char const* ptr) :
            m_ptrRecord(ptr),
            version(ptr),
            folderCount(ptr),
            accountUnlocked(ptr),
            unlockAccountDate(ptr),
            playerName(ptr),
            numBeatmaps(ptr),
            beatmaps(GetArray<Beatmap>(ptr, numBeatmaps)),
            userPermission(static_cast<UserPermission>(*reinterpret_cast<unsigned char const*>(ptr++)))
        {
            m_bytes = std::distance(m_ptrRecord, ptr);
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

        void write(unsigned char*& ptr) const
        {
            version.write(ptr);
            folderCount.write(ptr);
            accountUnlocked.write(ptr);
            unlockAccountDate.write(ptr);
            playerName.write(ptr);
            numBeatmaps.write(ptr);
            WriteArrayWithoutCount(ptr, beatmaps);
            TrivialParsable<unsigned char>{static_cast<unsigned char>(userPermission)}.write(ptr);
        }

        auto bytes() const
        {
            return m_bytes.value_or(version.bytes() +
                folderCount.bytes() +
                accountUnlocked.bytes() +
                unlockAccountDate.bytes() +
                playerName.bytes() +
                numBeatmaps.bytes() +
                GetArrayBytesWithoutCount(beatmaps) +
                1
            );
        }

        //auto getBuffer() const
        //{
        //    auto buffer = std::make_unique<unsigned char[]>(bytes());
        //    auto rawPtr = buffer.get();
        //    write(rawPtr);
        //    return buffer;
        //}

        auto getBeatmapSet() const
        {
            return std::unordered_set(beatmaps.cbegin(), beatmaps.cend());
        }

        auto getBeatmapMd5Map() const
        {
            std::unordered_map<std::string_view, Beatmap const*> map;
            std::transform(
                beatmaps.cbegin(),
                beatmaps.cend(),
                std::inserter(map, map.end()),
                [](Beatmap const& beatmap)
                {
                    return std::pair{ std::string_view{beatmap.md5}, &beatmap };
                }
            );
            return map;
        }
    };

    struct Collections : public ISerializable<Collections>
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

            void write(unsigned char*& ptr) const
            {
                name.write(ptr);
                WriteArrayWithCount(ptr, md5s);
            }

            auto bytes() const
            {
                return name.bytes() + GetArrayBytesWithCount(md5s);
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

        void write(unsigned char*& ptr) const
        {
            version.write(ptr);
            WriteArrayWithCount(ptr, collections);
        }

        auto bytes() const
        {
            return version.bytes() + GetArrayBytesWithCount(collections);
        }
    };

    struct Scores : ISerializable<Scores>
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
                    mode(static_cast<Beatmap::ModeEnum>(static_cast<unsigned char>(Beatmap::Mode{ ptr }))),
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

                auto bytes() const
                {
                    return 1 +
                        version.bytes() +
                        beatmapMd5.bytes() +
                        playerName.bytes() +
                        replayMd5.bytes() +
                        num300.bytes() +
                        num100.bytes() +
                        num50.bytes() +
                        numGeki.bytes() +
                        numKatus.bytes() +
                        numMiss.bytes() +
                        replayScore.bytes() +
                        maxCombo.bytes() +
                        perfectCombo.bytes() +
                        mods.bytes() +
                        _.bytes() +
                        timestamp.bytes() +
                        __.bytes() +
                        scoreId.bytes() +
                        (additionalMod.has_value() ? additionalMod->bytes() : 0);
                }

                void write(unsigned char*& ptr) const
                {
                    TrivialParsable<unsigned char>{static_cast<unsigned char>(mode)}.write(ptr);
                    version.write(ptr);
                    beatmapMd5.write(ptr);
                    playerName.write(ptr);
                    replayMd5.write(ptr);
                    num300.write(ptr);
                    num100.write(ptr);
                    num50.write(ptr);
                    numGeki.write(ptr);
                    numKatus.write(ptr);
                    numMiss.write(ptr);
                    replayScore.write(ptr);
                    maxCombo.write(ptr);
                    perfectCombo.write(ptr);
                    mods.write(ptr);
                    _.write(ptr);
                    timestamp.write(ptr);
                    __.write(ptr);
                    scoreId.write(ptr);
                    if (additionalMod.has_value())
                        additionalMod->write(ptr);
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

            void write(unsigned char*& ptr) const
            {
                md5.write(ptr);
                WriteArrayWithCount(ptr, scores);
            }

            auto bytes() const
            {
                return md5.size() + GetArrayBytesWithCount(scores);
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

        auto bytes() const
        {
            return version.bytes() + GetArrayBytesWithCount(beatmapScores);
        }

        void write(unsigned char*& ptr) const
        {
            version.write(ptr);
            WriteArrayWithCount(ptr, beatmapScores);
        }
    };
};

