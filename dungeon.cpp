#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

class DungeonMap {
private:
    int32_t width_;
    int32_t height_;
    std::vector<int32_t> data_;

public:
    DungeonMap() : width_(32), height_(24), data_(32 * 24, 1) {}
    DungeonMap(int32_t width, int32_t height, int32_t defVal = 1)
    : width_(width), height_(height), data_(width * height, defVal) {}

    [[nodiscard]] int32_t width() const noexcept { return width_; }
    [[nodiscard]] int32_t height() const noexcept { return height_; }
    [[nodiscard]] size_t size() const noexcept { return data_.size(); }

    [[nodiscard]] bool inBounds(int32_t x, int32_t y) const noexcept {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    [[nodiscard]] int32_t get(int32_t x, int32_t y, int32_t fallback = 1) const noexcept {
        if (!inBounds(x, y)) return fallback;
        return data_[static_cast<size_t>(y * width_ + x)];
    }

    bool set(int32_t x, int32_t y, int32_t val) noexcept {
        if (!inBounds(x, y)) return false;
        data_[static_cast<size_t>(y * width_ + x)] = val;
        return true;
    }

    [[nodiscard]] int32_t& at(int32_t x, int32_t y) {
        return data_.at(static_cast<size_t>(y * width_ + x));
    }

    void fill(int32_t val) {
        std::fill(data_.begin(), data_.end(), val);
    }
};

enum class GameFsmState : uint8_t {
    Title,
    Playing,
    Paused,
    Victory,
    GameOver,
};

enum class GameFsmEvent : uint8_t {
    Start,
    Win,
    Die,
    Pause,
    Resume,
    Quit,
    Restart,
};

class GameFsm {
private:
    GameFsmState state_ = GameFsmState::Title;

public:
    GameFsm() = default;
    explicit GameFsm(GameFsmState init) : state_(init) {}

    [[nodiscard]] GameFsmState getState() const noexcept { return state_; }
    [[nodiscard]] bool is(GameFsmState s) const noexcept { return state_ == s; }

    bool transition(GameFsmEvent event) noexcept {
        switch (state_) {
            case GameFsmState::Title:
            switch (event) {
                case GameFsmEvent::Start:
                state_ = GameFsmState::Playing;
                return true;
                default: break;
            }
            break;
            case GameFsmState::Playing:
            switch (event) {
                case GameFsmEvent::Win:
                state_ = GameFsmState::Victory;
                return true;
                case GameFsmEvent::Die:
                state_ = GameFsmState::GameOver;
                return true;
                case GameFsmEvent::Pause:
                state_ = GameFsmState::Paused;
                return true;
                default: break;
            }
            break;
            case GameFsmState::Paused:
            switch (event) {
                case GameFsmEvent::Resume:
                state_ = GameFsmState::Playing;
                return true;
                case GameFsmEvent::Quit:
                state_ = GameFsmState::Title;
                return true;
                default: break;
            }
            break;
            case GameFsmState::Victory:
            switch (event) {
                case GameFsmEvent::Restart:
                state_ = GameFsmState::Title;
                return true;
                default: break;
            }
            break;
            case GameFsmState::GameOver:
            switch (event) {
                case GameFsmEvent::Restart:
                state_ = GameFsmState::Title;
                return true;
                default: break;
            }
            break;
            default: break;
        }
        return false;
    }
};

class Player {
private:
    float x_ = 2.5;
    float y_ = 2.5;
    int32_t hp_ = 100;
    int32_t max_hp_ = 100;
    int32_t mana_ = 60;
    int32_t max_mana_ = 60;
    int32_t gold_ = 0;
    int32_t keys_ = 0;
    int32_t score_ = 0;
    int32_t facing_ = 0;
    float attack_timer_ = 0.0;
    float dash_timer_ = 0.0;
    float invuln_timer_ = 0.0;
    bool has_wand_ = false;
    bool has_boots_ = false;
    int32_t slain_count_ = 0;

public:
    Player() = default;
    explicit Player(float x, float y, int32_t hp, int32_t max_hp, int32_t mana, int32_t max_mana, int32_t gold, int32_t keys, int32_t score, int32_t facing, float attack_timer, float dash_timer, float invuln_timer, bool has_wand, bool has_boots, int32_t slain_count)
    : x_(x), y_(y), hp_(hp), max_hp_(max_hp), mana_(mana), max_mana_(max_mana), gold_(gold), keys_(keys), score_(score), facing_(facing), attack_timer_(attack_timer), dash_timer_(dash_timer), invuln_timer_(invuln_timer), has_wand_(has_wand), has_boots_(has_boots), slain_count_(slain_count) {}

    [[nodiscard]] float getX() const noexcept {
        return x_;
    }

    void setX(float value) noexcept {
        x_ = value;
    }

    [[nodiscard]] float getY() const noexcept {
        return y_;
    }

    void setY(float value) noexcept {
        y_ = value;
    }

    [[nodiscard]] int32_t getHp() const noexcept {
        return hp_;
    }

    void setHp(int32_t value) noexcept {
        hp_ = value;
    }

    [[nodiscard]] int32_t getMaxHp() const noexcept {
        return max_hp_;
    }

    void setMaxHp(int32_t value) noexcept {
        max_hp_ = value;
    }

    [[nodiscard]] int32_t getMana() const noexcept {
        return mana_;
    }

    void setMana(int32_t value) noexcept {
        mana_ = value;
    }

    [[nodiscard]] int32_t getMaxMana() const noexcept {
        return max_mana_;
    }

    void setMaxMana(int32_t value) noexcept {
        max_mana_ = value;
    }

    [[nodiscard]] int32_t getGold() const noexcept {
        return gold_;
    }

    void setGold(int32_t value) noexcept {
        gold_ = value;
    }

    [[nodiscard]] int32_t getKeys() const noexcept {
        return keys_;
    }

    void setKeys(int32_t value) noexcept {
        keys_ = value;
    }

    [[nodiscard]] int32_t getScore() const noexcept {
        return score_;
    }

    void setScore(int32_t value) noexcept {
        score_ = value;
    }

    [[nodiscard]] int32_t getFacing() const noexcept {
        return facing_;
    }

    void setFacing(int32_t value) noexcept {
        facing_ = value;
    }

    [[nodiscard]] float getAttackTimer() const noexcept {
        return attack_timer_;
    }

    void setAttackTimer(float value) noexcept {
        attack_timer_ = value;
    }

    [[nodiscard]] float getDashTimer() const noexcept {
        return dash_timer_;
    }

    void setDashTimer(float value) noexcept {
        dash_timer_ = value;
    }

    [[nodiscard]] float getInvulnTimer() const noexcept {
        return invuln_timer_;
    }

    void setInvulnTimer(float value) noexcept {
        invuln_timer_ = value;
    }

    [[nodiscard]] bool getHasWand() const noexcept {
        return has_wand_;
    }

    void setHasWand(bool value) noexcept {
        has_wand_ = value;
    }

    [[nodiscard]] bool getHasBoots() const noexcept {
        return has_boots_;
    }

    void setHasBoots(bool value) noexcept {
        has_boots_ = value;
    }

    [[nodiscard]] int32_t getSlainCount() const noexcept {
        return slain_count_;
    }

    void setSlainCount(int32_t value) noexcept {
        slain_count_ = value;
    }
};

class Enemy {
private:
    float x_ = 0.0;
    float y_ = 0.0;
    int32_t hp_ = 30;
    int32_t max_hp_ = 30;
    int32_t type_id_ = 0;
    bool alive_ = true;
    float speed_ = 0.035;
    int32_t facing_ = 0;
    float hit_timer_ = 0.0;
    float attack_cooldown_ = 0.0;

public:
    Enemy() = default;
    explicit Enemy(float x, float y, int32_t hp, int32_t max_hp, int32_t type_id, bool alive, float speed, int32_t facing, float hit_timer, float attack_cooldown)
    : x_(x), y_(y), hp_(hp), max_hp_(max_hp), type_id_(type_id), alive_(alive), speed_(speed), facing_(facing), hit_timer_(hit_timer), attack_cooldown_(attack_cooldown) {}

    [[nodiscard]] float getX() const noexcept {
        return x_;
    }

    void setX(float value) noexcept {
        x_ = value;
    }

    [[nodiscard]] float getY() const noexcept {
        return y_;
    }

    void setY(float value) noexcept {
        y_ = value;
    }

    [[nodiscard]] int32_t getHp() const noexcept {
        return hp_;
    }

    void setHp(int32_t value) noexcept {
        hp_ = value;
    }

    [[nodiscard]] int32_t getMaxHp() const noexcept {
        return max_hp_;
    }

    void setMaxHp(int32_t value) noexcept {
        max_hp_ = value;
    }

    [[nodiscard]] int32_t getTypeId() const noexcept {
        return type_id_;
    }

    void setTypeId(int32_t value) noexcept {
        type_id_ = value;
    }

    [[nodiscard]] bool getAlive() const noexcept {
        return alive_;
    }

    void setAlive(bool value) noexcept {
        alive_ = value;
    }

    [[nodiscard]] float getSpeed() const noexcept {
        return speed_;
    }

    void setSpeed(float value) noexcept {
        speed_ = value;
    }

    [[nodiscard]] int32_t getFacing() const noexcept {
        return facing_;
    }

    void setFacing(int32_t value) noexcept {
        facing_ = value;
    }

    [[nodiscard]] float getHitTimer() const noexcept {
        return hit_timer_;
    }

    void setHitTimer(float value) noexcept {
        hit_timer_ = value;
    }

    [[nodiscard]] float getAttackCooldown() const noexcept {
        return attack_cooldown_;
    }

    void setAttackCooldown(float value) noexcept {
        attack_cooldown_ = value;
    }
};

class Projectile {
private:
    float x_ = 0.0;
    float y_ = 0.0;
    float vx_ = 0.0;
    float vy_ = 0.0;
    bool is_enemy_ = false;
    bool alive_ = true;
    float life_ = 2.0;
    uint32_t color_ = 0;

public:
    Projectile() = default;
    explicit Projectile(float x, float y, float vx, float vy, bool is_enemy, bool alive, float life, uint32_t color)
    : x_(x), y_(y), vx_(vx), vy_(vy), is_enemy_(is_enemy), alive_(alive), life_(life), color_(color) {}

    [[nodiscard]] float getX() const noexcept {
        return x_;
    }

    void setX(float value) noexcept {
        x_ = value;
    }

    [[nodiscard]] float getY() const noexcept {
        return y_;
    }

    void setY(float value) noexcept {
        y_ = value;
    }

    [[nodiscard]] float getVx() const noexcept {
        return vx_;
    }

    void setVx(float value) noexcept {
        vx_ = value;
    }

    [[nodiscard]] float getVy() const noexcept {
        return vy_;
    }

    void setVy(float value) noexcept {
        vy_ = value;
    }

    [[nodiscard]] bool getIsEnemy() const noexcept {
        return is_enemy_;
    }

    void setIsEnemy(bool value) noexcept {
        is_enemy_ = value;
    }

    [[nodiscard]] bool getAlive() const noexcept {
        return alive_;
    }

    void setAlive(bool value) noexcept {
        alive_ = value;
    }

    [[nodiscard]] float getLife() const noexcept {
        return life_;
    }

    void setLife(float value) noexcept {
        life_ = value;
    }

    [[nodiscard]] uint32_t getColor() const noexcept {
        return color_;
    }

    void setColor(uint32_t value) noexcept {
        color_ = value;
    }
};

class Particle {
private:
    float x_ = 0.0;
    float y_ = 0.0;
    float vx_ = 0.0;
    float vy_ = 0.0;
    uint32_t color_ = 0;
    float life_ = 1.0;
    float max_life_ = 1.0;
    float size_ = 2.0;

public:
    Particle() = default;
    explicit Particle(float x, float y, float vx, float vy, uint32_t color, float life, float max_life, float size)
    : x_(x), y_(y), vx_(vx), vy_(vy), color_(color), life_(life), max_life_(max_life), size_(size) {}

    [[nodiscard]] float getX() const noexcept {
        return x_;
    }

    void setX(float value) noexcept {
        x_ = value;
    }

    [[nodiscard]] float getY() const noexcept {
        return y_;
    }

    void setY(float value) noexcept {
        y_ = value;
    }

    [[nodiscard]] float getVx() const noexcept {
        return vx_;
    }

    void setVx(float value) noexcept {
        vx_ = value;
    }

    [[nodiscard]] float getVy() const noexcept {
        return vy_;
    }

    void setVy(float value) noexcept {
        vy_ = value;
    }

    [[nodiscard]] uint32_t getColor() const noexcept {
        return color_;
    }

    void setColor(uint32_t value) noexcept {
        color_ = value;
    }

    [[nodiscard]] float getLife() const noexcept {
        return life_;
    }

    void setLife(float value) noexcept {
        life_ = value;
    }

    [[nodiscard]] float getMaxLife() const noexcept {
        return max_life_;
    }

    void setMaxLife(float value) noexcept {
        max_life_ = value;
    }

    [[nodiscard]] float getSize() const noexcept {
        return size_;
    }

    void setSize(float value) noexcept {
        size_ = value;
    }
};

void setPixel(int x, int y, uint32_t color);
void clearScreen(uint32_t color);
void drawRect(int x, int y, int w, int h, uint32_t color);
bool isKeyDown(int key);
bool isKeyPressed(std::string_view key);

static inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
}
static inline uint8_t getR(uint32_t c) { return (c >> 16) & 0xFF; }
static inline uint8_t getG(uint32_t c) { return (c >> 8) & 0xFF; }
static inline uint8_t getB(uint32_t c) { return c & 0xFF; }

static inline uint32_t multColor(uint32_t base, float lr, float lg, float lb) {
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, getR(base) * lr));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, getG(base) * lg));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, getB(base) * lb));
    return rgb(r, g, b);
}

static const uint8_t FONT_5X7[95][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x5F, 0x00, 0x00},
    {0x00, 0x07, 0x00, 0x07, 0x00},
    {0x14, 0x7F, 0x14, 0x7F, 0x14},
    {0x24, 0x2A, 0x7F, 0x2A, 0x12},
    {0x23, 0x13, 0x08, 0x64, 0x62},
    {0x36, 0x49, 0x55, 0x22, 0x50},
    {0x00, 0x05, 0x03, 0x00, 0x00},
    {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00},
    {0x14, 0x08, 0x3E, 0x08, 0x14},
    {0x08, 0x08, 0x3E, 0x08, 0x08},
    {0x00, 0x50, 0x30, 0x00, 0x00},
    {0x08, 0x08, 0x08, 0x08, 0x08},
    {0x00, 0x60, 0x60, 0x00, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02},
    {0x3E, 0x51, 0x49, 0x45, 0x3E},
    {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4B, 0x31},
    {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x30},
    {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1E},
    {0x00, 0x36, 0x36, 0x00, 0x00},
    {0x00, 0x56, 0x36, 0x00, 0x00},
    {0x08, 0x14, 0x22, 0x41, 0x00},
    {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x00, 0x41, 0x22, 0x14, 0x08},
    {0x02, 0x01, 0x51, 0x09, 0x06},
    {0x32, 0x49, 0x79, 0x41, 0x3E},
    {0x7E, 0x11, 0x11, 0x11, 0x7E},
    {0x7F, 0x49, 0x49, 0x49, 0x36},
    {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C},
    {0x7F, 0x49, 0x49, 0x49, 0x41},
    {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x49, 0x49, 0x7A},
    {0x7F, 0x08, 0x08, 0x08, 0x7F},
    {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01},
    {0x7F, 0x08, 0x14, 0x22, 0x41},
    {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    {0x7F, 0x04, 0x08, 0x10, 0x7F},
    {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06},
    {0x3E, 0x41, 0x51, 0x21, 0x5E},
    {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x46, 0x49, 0x49, 0x49, 0x31},
    {0x01, 0x01, 0x7F, 0x01, 0x01},
    {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F},
    {0x3F, 0x40, 0x38, 0x40, 0x3F},
    {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x07, 0x08, 0x70, 0x08, 0x07},
    {0x61, 0x51, 0x49, 0x45, 0x43},
    {0x00, 0x7F, 0x41, 0x41, 0x00},
    {0x02, 0x04, 0x08, 0x10, 0x20},
    {0x00, 0x41, 0x41, 0x7F, 0x00},
    {0x04, 0x02, 0x01, 0x02, 0x04},
    {0x40, 0x40, 0x40, 0x40, 0x40},
    {0x00, 0x01, 0x02, 0x04, 0x00},
    {0x20, 0x54, 0x54, 0x54, 0x78},
    {0x7F, 0x48, 0x44, 0x44, 0x38},
    {0x38, 0x44, 0x44, 0x44, 0x20},
    {0x38, 0x44, 0x44, 0x48, 0x7F},
    {0x38, 0x54, 0x54, 0x54, 0x18},
    {0x08, 0x7E, 0x09, 0x01, 0x02},
    {0x0C, 0x52, 0x52, 0x52, 0x3E},
    {0x7F, 0x08, 0x04, 0x04, 0x78},
    {0x00, 0x44, 0x7D, 0x40, 0x00},
    {0x20, 0x40, 0x44, 0x3D, 0x00},
    {0x7F, 0x10, 0x28, 0x44, 0x00},
    {0x00, 0x41, 0x7F, 0x40, 0x00},
    {0x7C, 0x04, 0x18, 0x04, 0x78},
    {0x7C, 0x08, 0x04, 0x04, 0x78},
    {0x38, 0x44, 0x44, 0x44, 0x38},
    {0x7C, 0x14, 0x14, 0x14, 0x08},
    {0x08, 0x14, 0x14, 0x18, 0x7C},
    {0x7C, 0x08, 0x04, 0x04, 0x08},
    {0x48, 0x54, 0x54, 0x54, 0x20},
    {0x04, 0x3F, 0x44, 0x40, 0x20},
    {0x3C, 0x40, 0x40, 0x20, 0x7C},
    {0x1C, 0x20, 0x40, 0x20, 0x1C},
    {0x3C, 0x40, 0x30, 0x40, 0x3C},
    {0x44, 0x28, 0x10, 0x28, 0x44},
    {0x0C, 0x50, 0x50, 0x50, 0x3C},
    {0x44, 0x64, 0x54, 0x4C, 0x44},
    {0x00, 0x08, 0x36, 0x41, 0x00},
    {0x00, 0x00, 0x7F, 0x00, 0x00},
    {0x00, 0x41, 0x36, 0x08, 0x00},
    {0x08, 0x08, 0x2A, 0x1C, 0x08}
};

static void drawChar(int x, int y, char c, uint32_t color, int scale = 1) {
    if (c < 32 || c > 126) c = '?';
    const uint8_t* glyph = FONT_5X7[c - 32];
    for (int col = 0; col < 5; ++col) {
        uint8_t bits = glyph[col];
        for (int row = 0; row < 7; ++row) {
            if ((bits >> row) & 1) {
                if (scale == 1) {
                    setPixel(x + col, y + row, color);
                } else {
                    drawRect(x + col * scale, y + row * scale, scale, scale, color);
                }
            }
        }
    }
}

static void drawString(int x, int y, const std::string& str, uint32_t color, int scale = 1, bool shadow = true) {
    int cx = x;
    for (char c : str) {
        if (c == '\n') {
            y += 9 * scale;
            cx = x;
            continue;
        }
        if (shadow) {
            drawChar(cx + scale, y + scale, c, 0x000000, scale);
        }
        drawChar(cx, y, c, color, scale);
        cx += 6 * scale;
    }
}

static constexpr int MAP_W = 32;
static constexpr int MAP_H = 24;
static constexpr int TILE_SIZE = 28;
static constexpr int SCREEN_W = 800;
static constexpr int SCREEN_H = 600;
static constexpr int HUD_H = 44;

// tu trzymany jest stan całej gry
static GameFsm g_fsm;
static DungeonMap g_map(MAP_W, MAP_H, 1);
static Player g_player;
static std::vector<Enemy> g_enemies;
static std::vector<Particle> g_particles;
static std::vector<Projectile> g_projectiles;

struct TorchLight {
    float x, y;
    float radius;
    float r, g, b;
};
static std::vector<TorchLight> g_torches;

struct FloatingText {
    float x, y;
    std::string text;
    uint32_t color;
    float life;
};
static std::vector<FloatingText> g_floaters;

static float g_gameTime = 0.0f;
static float g_shakeTimer = 0.0f;
static int g_shakeX = 0;
static int g_shakeY = 0;

static float g_slashX = 0.0f;
static float g_slashY = 0.0f;
static int g_slashDir = 0;
static float g_slashTimer = 0.0f;

static void addFloater(float x, float y, const std::string& text, uint32_t color) {
    g_floaters.push_back({x, y, text, color, 1.0f});
}

static void emitParticles(float x, float y, uint32_t color, int count, float speedMult = 1.0f) {
    for (int i = 0; i < count; ++i) {
        float angle = static_cast<float>(rand() % 628) / 100.0f;
        float speed = (1.0f + static_cast<float>(rand() % 25) / 10.0f) * speedMult;
        Particle p;
        p.setX(x);
        p.setY(y);
        p.setVx(std::cos(angle) * speed);
        p.setVy(std::sin(angle) * speed);
        p.setColor(color);
        p.setLife(0.4f + static_cast<float>(rand() % 30) / 100.0f);
        p.setMaxLife(p.getLife());
        p.setSize(1.5f + static_cast<float>(rand() % 2));
        g_particles.push_back(p);
    }
}

static void carveRect(int rx, int ry, int rw, int rh) {
    for (int y = ry; y < ry + rh; ++y) {
        for (int x = rx; x < rx + rw; ++x) {
            if (x > 0 && x < MAP_W - 1 && y > 0 && y < MAP_H - 1) {
                g_map.set(x, y, 2);
            }
        }
    }
}

static void carveHCorridor(int x1, int x2, int y) {
    int start = std::min(x1, x2);
    int end = std::max(x1, x2);
    for (int x = start; x <= end; ++x) {
        if (x > 0 && x < MAP_W - 1 && y > 0 && y < MAP_H - 1) {
            g_map.set(x, y, 2);
        }
    }
}

static void carveVCorridor(int y1, int y2, int x) {
    int start = std::min(y1, y2);
    int end = std::max(y1, y2);
    for (int y = start; y <= end; ++y) {
        if (x > 0 && x < MAP_W - 1 && y > 0 && y < MAP_H - 1) {
            g_map.set(x, y, 2);
        }
    }
}

static void spawnEnemy(float ex, float ey, int type, int hp) {
    Enemy e;
    e.setX(ex);
    e.setY(ey);
    e.setTypeId(type);
    e.setHp(hp);
    e.setMaxHp(hp);
    e.setAlive(true);
    e.setSpeed(type == 0 ? 0.034f : (type == 1 ? 0.025f : (type == 2 ? 0.022f : 0.020f)));
    g_enemies.push_back(e);
}

// układ mapy i miejsca startowe są ustawiane tutaj
static void initDungeon() {
    g_map.fill(1);
    g_enemies.clear();
    g_particles.clear();
    g_projectiles.clear();
    g_torches.clear();
    g_floaters.clear();

    carveRect(1, 1, 6, 6);
    carveRect(11, 2, 7, 5);
    carveRect(22, 2, 8, 6);
    carveRect(2, 10, 7, 6);
    carveRect(12, 10, 8, 7);
    carveRect(22, 12, 8, 9);
    carveRect(2, 18, 6, 5);

    carveHCorridor(6, 11, 3);
    carveHCorridor(17, 22, 4);
    carveVCorridor(6, 10, 4);
    carveHCorridor(8, 12, 13);
    carveVCorridor(6, 11, 15);
    carveHCorridor(19, 22, 15);
    carveVCorridor(15, 19, 4);
    carveHCorridor(7, 14, 20);
    carveVCorridor(16, 20, 14);

    g_map.set(22, 15, 3);
    g_map.set(28, 3, 4);

    g_map.set(13, 3, 11);
    g_map.set(3, 19, 12);

    g_map.set(5, 2, 13);
    g_map.set(2, 5, 13);
    g_map.set(12, 4, 13);
    g_map.set(16, 11, 13);
    g_map.set(23, 14, 13);
    g_map.set(28, 13, 13);
    g_map.set(5, 21, 13);

    g_map.set(2, 2, 5);
    g_map.set(27, 4, 5);
    g_map.set(28, 14, 5);

    g_map.set(14, 4, 6);
    g_map.set(4, 12, 6);
    g_map.set(5, 14, 6);
    g_map.set(24, 18, 6);

    g_map.set(15, 4, 7);
    g_map.set(4, 21, 7);
    g_map.set(15, 15, 7);

    g_map.set(9, 3, 8);
    g_map.set(10, 3, 8);
    g_map.set(10, 13, 8);
    g_map.set(11, 13, 8);

    g_map.set(28, 19, 9);

    auto addTorch = [](int tx, int ty) {
        g_map.set(tx, ty, 10);
        g_torches.push_back({(tx + 0.5f) * TILE_SIZE, (ty + 0.5f) * TILE_SIZE, 140.0f, 1.0f, 0.75f, 0.40f});
    };
    addTorch(4, 1);
    addTorch(14, 2);
    addTorch(25, 2);
    addTorch(4, 10);
    addTorch(16, 10);
    addTorch(25, 12);
    addTorch(4, 18);

    g_player.setX(2.5f);
    g_player.setY(2.5f);
    g_player.setHp(100);
    g_player.setMaxHp(100);
    g_player.setMana(60);
    g_player.setMaxMana(60);
    g_player.setGold(0);
    g_player.setKeys(0);
    g_player.setScore(0);
    g_player.setFacing(0);
    g_player.setAttackTimer(0.0f);
    g_player.setDashTimer(0.0f);
    g_player.setInvulnTimer(0.0f);
    g_player.setHasWand(false);
    g_player.setHasBoots(false);
    g_player.setSlainCount(0);

    spawnEnemy(4.5f, 13.5f, 1, 30);
    spawnEnemy(6.0f, 12.0f, 1, 30);
    spawnEnemy(14.5f, 4.0f, 0, 35);
    spawnEnemy(25.5f, 5.0f, 0, 35);
    spawnEnemy(15.5f, 13.5f, 0, 35);
    spawnEnemy(25.5f, 17.5f, 2, 140);
}

static inline bool isSolid(int tx, int ty) {
    if (tx < 0 || tx >= MAP_W || ty < 0 || ty >= MAP_H) return true;
    int t = g_map.get(tx, ty, 1);
    return t == 1 || t == 3 || t == 10 || t == 13;
}

static void drawStoneWall(int px, int py, float lr, float lg, float lb) {
    for (int dy = 0; dy < TILE_SIZE; ++dy) {
        int rowInBrick = dy % 7;
        bool isMortarH = (rowInBrick == 0);
        int brickRow = dy / 7;
        int xOffset = (brickRow % 2 == 0) ? 0 : 7;

        for (int dx = 0; dx < TILE_SIZE; ++dx) {
            int colInBrick = (dx + xOffset) % 14;
            bool isMortarV = (colInBrick == 0);

            uint32_t base;
            if (isMortarH || isMortarV) {
                base = rgb(20, 22, 32);
            } else if (rowInBrick == 1) {
                base = rgb(70, 75, 95);
            } else if (rowInBrick == 6) {
                base = rgb(26, 28, 40);
            } else {
                int noise = ((dx * 13 + dy * 17) % 7) - 3;
                base = rgb(45 + noise, 48 + noise, 64 + noise);
            }
            setPixel(px + dx, py + dy, multColor(base, lr, lg, lb));
        }
    }
}

static void drawFloor(int px, int py, float lr, float lg, float lb) {
    for (int dy = 0; dy < TILE_SIZE; ++dy) {
        for (int dx = 0; dx < TILE_SIZE; ++dx) {
            bool isGrid = (dx == 0 || dy == 0 || dx == TILE_SIZE - 1 || dy == TILE_SIZE - 1);
            uint32_t base;
            if (isGrid) {
                base = rgb(18, 20, 28);
            } else {
                int speck = ((dx * 7 + dy * 11) % 5);
                base = rgb(28 + speck, 30 + speck, 42 + speck);
            }
            setPixel(px + dx, py + dy, multColor(base, lr, lg, lb));
        }
    }
}

static void drawTorchTile(int px, int py, float lr, float lg, float lb) {
    drawStoneWall(px, py, lr, lg, lb);
    drawRect(px + 12, py + 14, 4, 7, rgb(30, 30, 35));
    float flk = std::sin(g_gameTime * 20.0f + px);
    int fh = 6 + static_cast<int>(flk * 2.0f);
    drawRect(px + 11, py + 14 - fh, 6, fh, rgb(255, 140, 20));
    drawRect(px + 12, py + 15 - fh, 4, fh - 2, rgb(255, 230, 80));
    drawRect(px + 13, py + 16 - fh, 2, fh - 4, rgb(255, 255, 200));
}

static void drawDoor(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    for (int bx = 2; bx < TILE_SIZE; bx += 5) {
        drawRect(px + bx, py, 2, TILE_SIZE, rgb(80, 90, 105));
    }
    drawRect(px, py + 4, TILE_SIZE, 3, rgb(60, 70, 85));
    drawRect(px, py + TILE_SIZE - 6, TILE_SIZE, 3, rgb(60, 70, 85));
    drawRect(px + 11, py + 10, 6, 8, rgb(255, 195, 0));
    drawRect(px + 13, py + 13, 2, 3, rgb(40, 40, 40));
}

static void drawKeyItem(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int bob = static_cast<int>(std::sin(g_gameTime * 6.0f) * 2.0f);
    int kx = px + 8;
    int ky = py + 8 + bob;
    drawRect(kx, ky, 6, 6, rgb(255, 215, 0));
    drawRect(kx + 2, ky + 2, 2, 2, rgb(30, 30, 40));
    drawRect(kx + 5, ky + 2, 7, 2, rgb(255, 215, 0));
    drawRect(kx + 9, ky + 4, 2, 3, rgb(255, 215, 0));
}

static void drawChest(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int cx = px + 4;
    int cy = py + 6;
    drawRect(cx, cy, 20, 15, rgb(110, 55, 25));
    drawRect(cx, cy, 3, 15, rgb(255, 200, 30));
    drawRect(cx + 17, cy, 3, 15, rgb(255, 200, 30));
    drawRect(cx, cy + 5, 20, 3, rgb(255, 200, 30));
    drawRect(cx + 9, cy + 6, 3, 4, rgb(230, 20, 40));
}

static void drawGem(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int gx = px + 10;
    int gy = py + 9 + static_cast<int>(std::sin(g_gameTime * 7.0f) * 2.0f);
    drawRect(gx + 2, gy, 4, 2, rgb(255, 80, 110));
    drawRect(gx + 1, gy + 2, 6, 3, rgb(255, 25, 60));
    drawRect(gx + 2, gy + 5, 4, 3, rgb(180, 10, 40));
    drawRect(gx + 3, gy + 8, 2, 2, rgb(120, 5, 25));
    setPixel(gx + 2, gy + 2, rgb(255, 255, 255));
}

static void drawPotion(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int px_ = px + 9;
    int py_ = py + 7 + static_cast<int>(std::sin(g_gameTime * 5.0f) * 2.0f);
    drawRect(px_ + 3, py_, 4, 2, rgb(160, 110, 70));
    drawRect(px_ + 4, py_ + 2, 2, 3, rgb(180, 220, 240));
    drawRect(px_ + 1, py_ + 5, 8, 8, rgb(235, 30, 60));
    drawRect(px_ + 2, py_ + 4, 6, 1, rgb(235, 30, 60));
    setPixel(px_ + 2, py_ + 6, rgb(255, 255, 255));
}

static void drawWandItem(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int wx = px + 11;
    int wy = py + 6 + static_cast<int>(std::sin(g_gameTime * 6.0f) * 2.0f);
    drawRect(wx + 2, wy + 5, 2, 12, rgb(130, 80, 30));
    drawRect(wx + 1, wy + 4, 4, 2, rgb(210, 160, 40));
    drawRect(wx, wy, 6, 5, rgb(255, 100, 20));
    drawRect(wx + 1, wy + 1, 4, 3, rgb(255, 240, 80));
}

static void drawBootsItem(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int bx = px + 8;
    int by = py + 8 + static_cast<int>(std::sin(g_gameTime * 6.0f) * 2.0f);
    drawRect(bx, by + 4, 5, 6, rgb(40, 160, 240));
    drawRect(bx + 7, by + 4, 5, 6, rgb(40, 160, 240));
    drawRect(bx - 2, by + 2, 3, 4, rgb(255, 255, 255));
    drawRect(bx + 11, by + 2, 3, 4, rgb(255, 255, 255));
}

static void drawPotItem(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    int px_ = px + 7;
    int py_ = py + 6;
    drawRect(px_ + 3, py_, 8, 2, rgb(180, 100, 50));
    drawRect(px_ + 1, py_ + 2, 12, 13, rgb(160, 85, 40));
    drawRect(px_ + 3, py_ + 15, 8, 2, rgb(130, 65, 30));
    setPixel(px_ + 3, py_ + 4, rgb(220, 140, 80));
}

static void drawSpikes(int px, int py, float lr, float lg, float lb) {
    drawFloor(px, py, lr, lg, lb);
    drawRect(px + 4, py + 4, 20, 20, rgb(15, 16, 22));
    int offsets[4][2] = {{6, 6}, {16, 6}, {6, 16}, {16, 16}};
    for (int i = 0; i < 4; ++i) {
        int sx = px + offsets[i][0];
        int sy = py + offsets[i][1];
        drawRect(sx + 2, sy, 2, 2, rgb(240, 245, 255));
        drawRect(sx + 1, sy + 2, 4, 2, rgb(180, 190, 205));
        drawRect(sx, sy + 4, 6, 2, rgb(100, 110, 125));
    }
}

static void drawPortal(int px, int py) {
    int cx = px + TILE_SIZE / 2;
    int cy = py + TILE_SIZE / 2;
    float t = g_gameTime * 4.0f;
    for (int r = 12; r >= 1; --r) {
        float angleOffset = t + r * 0.4f;
        uint32_t col = (r % 2 == 0) ? rgb(0, 230, 255) : rgb(180, 40, 255);
        if (r <= 3) col = rgb(255, 255, 255);
        for (int a = 0; a < 8; ++a) {
            float ang = angleOffset + a * (3.14159f / 4.0f);
            int sx = cx + static_cast<int>(std::cos(ang) * r);
            int sy = cy + static_cast<int>(std::sin(ang) * r);
            setPixel(sx, sy, col);
        }
    }
}

static void drawPlayerSprite(int sx, int sy, int facing, bool flash) {
    uint32_t armorColor = flash ? rgb(255, 255, 255) : rgb(70, 130, 210);
    uint32_t visorColor = flash ? rgb(255, 100, 100) : rgb(0, 240, 255);
    uint32_t helmColor = flash ? rgb(255, 255, 255) : rgb(180, 195, 215);

    drawRect(sx + 2, sy + 18, 16, 4, rgb(10, 12, 18));

    drawRect(sx + 4, sy + 8, 12, 10, armorColor);
    drawRect(sx + 6, sy + 18, 3, 4, rgb(50, 60, 80));
    drawRect(sx + 11, sy + 18, 3, 4, rgb(50, 60, 80));

    drawRect(sx + 5, sy + 1, 10, 8, helmColor);
    if (facing == 0) drawRect(sx + 7, sy + 4, 6, 2, visorColor);
    else if (facing == 1) drawRect(sx + 7, sy + 2, 6, 2, rgb(60, 70, 85));
    else if (facing == 2) drawRect(sx + 5, sy + 4, 4, 2, visorColor);
    else if (facing == 3) drawRect(sx + 11, sy + 4, 4, 2, visorColor);

    if (facing == 3) {
        drawRect(sx + 16, sy + 6, 6, 2, rgb(240, 245, 255));
        drawRect(sx + 15, sy + 5, 2, 4, rgb(200, 160, 40));
    } else if (facing == 2) {
        drawRect(sx - 2, sy + 6, 6, 2, rgb(240, 245, 255));
        drawRect(sx + 3, sy + 5, 2, 4, rgb(200, 160, 40));
    } else {
        drawRect(sx + 15, sy + 6, 2, 8, rgb(240, 245, 255));
        drawRect(sx + 14, sy + 6, 4, 2, rgb(200, 160, 40));
    }

    if (g_player.getHasWand()) {
        drawRect(sx + 1, sy + 4, 2, 8, rgb(180, 100, 30));
        setPixel(sx + 1, sy + 3, rgb(255, 120, 20));
    }
}

static void drawEnemySprite(const Enemy& e, int sx, int sy) {
    bool flash = e.getHitTimer() > 0.0f;
    int type = e.getTypeId();

    if (type == 0) {
        uint32_t bone = flash ? rgb(255, 255, 255) : rgb(220, 225, 230);
        drawRect(sx + 5, sy + 2, 10, 8, bone);
        drawRect(sx + 7, sy + 5, 2, 2, rgb(255, 20, 40));
        drawRect(sx + 11, sy + 5, 2, 2, rgb(255, 20, 40));
        drawRect(sx + 7, sy + 10, 6, 7, bone);
        drawRect(sx + 6, sy + 17, 2, 5, bone);
        drawRect(sx + 12, sy + 17, 2, 5, bone);
        drawRect(sx + 15, sy + 8, 2, 6, rgb(140, 150, 160));
    } else if (type == 1) {
        uint32_t green = flash ? rgb(255, 255, 255) : rgb(0, 230, 110);
        int squash = static_cast<int>(std::sin(g_gameTime * 10.0f + sx) * 2.0f);
        drawRect(sx + 3 - squash, sy + 6 + squash, 14 + squash * 2, 14 - squash, green);
        drawRect(sx + 6, sy + 8 + squash, 2, 3, rgb(255, 255, 255));
        drawRect(sx + 7, sy + 11 + squash, 2, 2, rgb(20, 40, 20));
        drawRect(sx + 11, sy + 11 + squash, 2, 2, rgb(20, 40, 20));
    } else if (type == 3) {
        uint32_t green = flash ? rgb(255, 255, 255) : rgb(100, 255, 150);
        drawRect(sx + 5, sy + 12, 10, 8, green);
        setPixel(sx + 7, sy + 14, rgb(20, 40, 20));
        setPixel(sx + 11, sy + 14, rgb(20, 40, 20));
    } else if (type == 2) {
        uint32_t demonRed = flash ? rgb(255, 255, 255) : rgb(190, 20, 30);
        drawRect(sx + 3, sy - 2, 3, 5, rgb(30, 30, 35));
        drawRect(sx + 18, sy - 2, 3, 5, rgb(30, 30, 35));
        drawRect(sx + 5, sy + 3, 14, 10, demonRed);
        drawRect(sx + 7, sy + 6, 3, 2, rgb(255, 235, 0));
        drawRect(sx + 14, sy + 6, 3, 2, rgb(255, 235, 0));
        drawRect(sx + 4, sy + 13, 16, 12, demonRed);
        drawRect(sx + 1, sy + 13, 4, 5, rgb(60, 60, 65));
        drawRect(sx + 19, sy + 13, 4, 5, rgb(60, 60, 65));
        drawRect(sx + 22, sy + 6, 3, 18, rgb(120, 120, 130));
        drawRect(sx + 20, sy + 4, 7, 6, rgb(220, 40, 20));
    }

    int barW = (type == 2 ? 28 : 16);
    int barX = sx + (TILE_SIZE - barW) / 2;
    int barY = sy - 6;
    float hpFrac = std::max(0.0f, std::min(1.0f, static_cast<float>(e.getHp()) / static_cast<float>(e.getMaxHp())));
    drawRect(barX - 1, barY - 1, barW + 2, 4, rgb(20, 20, 25));
    drawRect(barX, barY, static_cast<int>(barW * hpFrac), 2, type == 2 ? rgb(255, 40, 50) : rgb(0, 230, 100));
}

// jedna klatka: wejście, ruch, walka i rysowanie
void updateGame() {
    static bool s_initialized = false;
    if (!s_initialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        initDungeon();
        s_initialized = true;
    }

    g_gameTime += 0.016f;

    if (g_fsm.is(GameFsmState::Title)) {
        clearScreen(rgb(10, 12, 20));
        for (int y = 0; y < SCREEN_H; y += 40) drawRect(0, y, SCREEN_W, 1, rgb(18, 20, 32));
        for (int x = 0; x < SCREEN_W; x += 40) drawRect(x, 0, 1, SCREEN_H, rgb(18, 20, 32));

        drawString(210, 150, "DUNGEON CRYPT", rgb(255, 215, 0), 4);
        drawString(250, 215, "2D ACTION MAZE CRAWLER", rgb(140, 180, 240), 1);

        drawRect(180, 280, 440, 70, rgb(20, 24, 38));
        drawString(200, 295, "[WASD / ARROWS] MOVE     [SPACE] SWORD ATTACK", rgb(220, 230, 245), 1);
        drawString(200, 320, "[F / E] FIREBALL (WAND)  [SHIFT] DASH (BOOTS)", rgb(220, 230, 245), 1);

        bool flash = (static_cast<int>(g_gameTime * 2.5f) % 2 == 0);
        if (flash) {
            drawString(280, 410, ">>> PRESS [SPACE] TO PLAY <<<", rgb(0, 240, 255), 1);
        }

        drawPlayerSprite(260, 480, 3, false);
        Enemy demoSkel; demoSkel.setTypeId(0); demoSkel.setHp(35); demoSkel.setMaxHp(35);
        drawEnemySprite(demoSkel, 360, 480);
        Enemy demoSlime; demoSlime.setTypeId(1); demoSlime.setHp(30); demoSlime.setMaxHp(30);
        drawEnemySprite(demoSlime, 440, 480);
        Enemy demoBoss; demoBoss.setTypeId(2); demoBoss.setHp(140); demoBoss.setMaxHp(140);
        drawEnemySprite(demoBoss, 520, 470);

        if (isKeyPressed("space") || isKeyDown(13) || isKeyPressed("w") || isKeyPressed("up")) {
            initDungeon();
            g_fsm.transition(GameFsmEvent::Start);
        }
        return;
    }

    if (g_fsm.is(GameFsmState::Victory)) {
        clearScreen(rgb(14, 25, 20));
        drawRect(180, 140, 440, 320, rgb(20, 35, 30));
        drawString(260, 180, "MAZE CONQUERED!", rgb(255, 225, 50), 3);
        drawString(240, 260, "GOLD LOOTED: " + std::to_string(g_player.getGold()), rgb(255, 215, 0), 2);
        drawString(240, 300, "FOES SLAIN:  " + std::to_string(g_player.getSlainCount()), rgb(255, 100, 120), 2);
        drawString(240, 340, "SCORE:       " + std::to_string(g_player.getScore()), rgb(0, 240, 255), 2);

        bool flash = (static_cast<int>(g_gameTime * 2.5f) % 2 == 0);
        if (flash) {
            drawString(270, 410, "[SPACE] PLAY AGAIN", rgb(255, 255, 255), 1);
        }
        if (isKeyPressed("space") || isKeyDown(13)) {
            initDungeon();
            g_fsm.transition(GameFsmEvent::Restart);
        }
        return;
    }

    if (g_fsm.is(GameFsmState::GameOver)) {
        clearScreen(rgb(25, 10, 12));
        drawRect(180, 140, 440, 320, rgb(35, 15, 20));
        drawString(300, 180, "YOU DIED", rgb(255, 40, 60), 3);
        drawString(240, 260, "GOLD:  " + std::to_string(g_player.getGold()), rgb(255, 215, 0), 2);
        drawString(240, 300, "KILLS: " + std::to_string(g_player.getSlainCount()), rgb(255, 120, 140), 2);
        drawString(240, 340, "SCORE: " + std::to_string(g_player.getScore()), rgb(180, 200, 240), 2);

        bool flash = (static_cast<int>(g_gameTime * 2.5f) % 2 == 0);
        if (flash) {
            drawString(290, 410, "[SPACE] RETRY", rgb(255, 255, 255), 1);
        }
        if (isKeyPressed("space") || isKeyDown(13)) {
            initDungeon();
            g_fsm.transition(GameFsmEvent::Restart);
        }
        return;
    }

    if (g_shakeTimer > 0.0f) {
        g_shakeTimer -= 0.016f;
        g_shakeX = (rand() % 9) - 4;
        g_shakeY = (rand() % 9) - 4;
    } else {
        g_shakeX = 0;
        g_shakeY = 0;
    }

    if (g_player.getInvulnTimer() > 0.0f) g_player.setInvulnTimer(g_player.getInvulnTimer() - 0.016f);
    if (g_player.getAttackTimer() > 0.0f) g_player.setAttackTimer(g_player.getAttackTimer() - 0.016f);
    if (g_player.getDashTimer() > 0.0f) g_player.setDashTimer(g_player.getDashTimer() - 0.016f);
    if (g_slashTimer > 0.0f) g_slashTimer -= 0.016f;

    if (g_player.getMana() < g_player.getMaxMana()) {
        static float manaTimer = 0.0f;
        manaTimer += 0.016f;
        if (manaTimer >= 0.5f) {
            g_player.setMana(std::min(g_player.getMaxMana(), g_player.getMana() + 2));
            manaTimer = 0.0f;
        }
    }

    float moveX = 0.0f;
    float moveY = 0.0f;
    if (isKeyDown('W') || isKeyDown(38)) { moveY -= 1.0f; g_player.setFacing(1); }
    if (isKeyDown('S') || isKeyDown(40)) { moveY += 1.0f; g_player.setFacing(0); }
    if (isKeyDown('A') || isKeyDown(37)) { moveX -= 1.0f; g_player.setFacing(2); }
    if (isKeyDown('D') || isKeyDown(39)) { moveX += 1.0f; g_player.setFacing(3); }

    if (moveX != 0.0f && moveY != 0.0f) {
        moveX *= 0.7071f;
        moveY *= 0.7071f;
    }

    float speed = g_player.getHasBoots() ? 0.11f : 0.095f;
    if ((isKeyDown(16) || isKeyDown('C')) && g_player.getHasBoots() && g_player.getDashTimer() <= 0.0f) {
        g_player.setDashTimer(0.8f);
        g_player.setInvulnTimer(0.25f);
        speed = 0.35f;
        emitParticles(g_player.getX() * TILE_SIZE, g_player.getY() * TILE_SIZE, rgb(80, 200, 255), 10);
    }

    float curX = g_player.getX();
    float curY = g_player.getY();
    float nextX = curX + moveX * speed;
    float nextY = curY + moveY * speed;

    float radius = 0.28f;
    if (!isSolid(static_cast<int>(nextX + radius), static_cast<int>(curY)) &&
    !isSolid(static_cast<int>(nextX - radius), static_cast<int>(curY))) {
        curX = nextX;
    }
    if (!isSolid(static_cast<int>(curX), static_cast<int>(nextY + radius)) &&
    !isSolid(static_cast<int>(curX), static_cast<int>(nextY - radius))) {
        curY = nextY;
    }
    g_player.setX(curX);
    g_player.setY(curY);

    if ((isKeyDown(' ') || isKeyDown(32)) && g_player.getAttackTimer() <= 0.0f) {
        g_player.setAttackTimer(0.24f);
        g_slashTimer = 0.18f;
        g_slashDir = g_player.getFacing();

        float ax = curX;
        float ay = curY;
        if (g_slashDir == 0) ay += 0.8f;
        else if (g_slashDir == 1) ay -= 0.8f;
        else if (g_slashDir == 2) ax -= 0.8f;
        else if (g_slashDir == 3) ax += 0.8f;
        g_slashX = ax;
        g_slashY = ay;

        int hitTileX = static_cast<int>(ax);
        int hitTileY = static_cast<int>(ay);
        if (g_map.get(hitTileX, hitTileY, 0) == 13) {
            g_map.set(hitTileX, hitTileY, 2);
            emitParticles((hitTileX + 0.5f) * TILE_SIZE, (hitTileY + 0.5f) * TILE_SIZE, rgb(180, 110, 50), 12);
            int drop = rand() % 3;
            if (drop == 0) {
                g_player.setGold(g_player.getGold() + 20);
                addFloater((hitTileX + 0.5f) * TILE_SIZE, (hitTileY + 0.5f) * TILE_SIZE, "+20 G", rgb(255, 215, 0));
            } else if (drop == 1 && g_player.getHp() < g_player.getMaxHp()) {
                g_player.setHp(std::min(g_player.getMaxHp(), g_player.getHp() + 20));
                addFloater((hitTileX + 0.5f) * TILE_SIZE, (hitTileY + 0.5f) * TILE_SIZE, "+20 HP", rgb(0, 255, 120));
            }
        }

        for (size_t i = 0; i < g_enemies.size(); ++i) {
            auto& e = g_enemies[i];
            if (!e.getAlive()) continue;
            float edist = std::hypot(e.getX() - ax, e.getY() - ay);
            if (edist < 1.35f) {
                int dmg = 40;
                int newHp = e.getHp() - dmg;
                e.setHp(newHp);
                e.setHitTimer(0.20f);
                addFloater(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE - 10, "-" + std::to_string(dmg), rgb(255, 240, 40));
                emitParticles(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE, rgb(255, 60, 80), 8);

                e.setX(e.getX() + (ax - curX) * 0.45f);
                e.setY(e.getY() + (ay - curY) * 0.45f);

                if (newHp <= 0) {
                    e.setHp(0);
                    e.setAlive(false);
                    g_player.setSlainCount(g_player.getSlainCount() + 1);
                    int pts = (e.getTypeId() == 2 ? 600 : 100);
                    g_player.setScore(g_player.getScore() + pts);
                    addFloater(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE - 20, "+" + std::to_string(pts), rgb(0, 255, 200));
                    emitParticles(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE, rgb(255, 215, 0), 16, 1.8f);

                    if (e.getTypeId() == 1) {
                        spawnEnemy(e.getX() + 0.25f, e.getY(), 3, 15);
                        spawnEnemy(e.getX() - 0.25f, e.getY(), 3, 15);
                    }
                }
            }
        }
    }

    static float fireCooldown = 0.0f;
    if (fireCooldown > 0.0f) fireCooldown -= 0.016f;

    if ((isKeyDown('F') || isKeyDown('E') || isKeyDown('X')) && g_player.getHasWand() && fireCooldown <= 0.0f) {
        if (g_player.getMana() >= 15) {
            g_player.setMana(g_player.getMana() - 15);
            fireCooldown = 0.35f;

            float fvx = 0.0f, fvy = 0.0f;
            int fdir = g_player.getFacing();
            if (fdir == 0) fvy = 0.22f;
            else if (fdir == 1) fvy = -0.22f;
            else if (fdir == 2) fvx = -0.22f;
            else if (fdir == 3) fvx = 0.22f;

            Projectile p;
            p.setX(curX);
            p.setY(curY);
            p.setVx(fvx);
            p.setVy(fvy);
            p.setLife(2.0f);
            p.setAlive(true);
            p.setColor(rgb(255, 120, 20));
            g_projectiles.push_back(p);
            emitParticles(curX * TILE_SIZE, curY * TILE_SIZE, rgb(255, 180, 40), 8);
        }
    }

    for (size_t i = 0; i < g_projectiles.size();) {
        auto& pr = g_projectiles[i];
        pr.setX(pr.getX() + pr.getVx());
        pr.setY(pr.getY() + pr.getVy());
        pr.setLife(pr.getLife() - 0.016f);

        if (rand() % 2 == 0) {
            emitParticles(pr.getX() * TILE_SIZE, pr.getY() * TILE_SIZE, rgb(255, 140, 30), 1, 0.4f);
        }

        int ptx = static_cast<int>(pr.getX());
        int pty = static_cast<int>(pr.getY());
        bool hitWall = isSolid(ptx, pty);

        bool hitEnemy = false;
        for (auto& e : g_enemies) {
            if (!e.getAlive()) continue;
            float dist = std::hypot(e.getX() - pr.getX(), e.getY() - pr.getY());
            if (dist < 0.75f) {
                hitEnemy = true;
                int dmg = 60;
                int newHp = e.getHp() - dmg;
                e.setHp(newHp);
                e.setHitTimer(0.25f);
                addFloater(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE - 10, "-" + std::to_string(dmg), rgb(255, 80, 20));
                if (newHp <= 0) {
                    e.setHp(0);
                    e.setAlive(false);
                    g_player.setSlainCount(g_player.getSlainCount() + 1);
                    int pts = (e.getTypeId() == 2 ? 600 : 100);
                    g_player.setScore(g_player.getScore() + pts);
                    addFloater(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE - 20, "+" + std::to_string(pts), rgb(0, 255, 200));
                    emitParticles(e.getX() * TILE_SIZE, e.getY() * TILE_SIZE, rgb(255, 215, 0), 16, 1.8f);
                    if (e.getTypeId() == 1) {
                        spawnEnemy(e.getX() + 0.25f, e.getY(), 3, 15);
                        spawnEnemy(e.getX() - 0.25f, e.getY(), 3, 15);
                    }
                }
                break;
            }
        }

        if (hitWall || hitEnemy || pr.getLife() <= 0.0f) {
            emitParticles(pr.getX() * TILE_SIZE, pr.getY() * TILE_SIZE, rgb(255, 120, 20), 14, 1.5f);
            g_projectiles.erase(g_projectiles.begin() + i);
        } else {
            ++i;
        }
    }

    int pTileX = static_cast<int>(curX);
    int pTileY = static_cast<int>(curY);
    int adj[4][2] = {{pTileX+1, pTileY}, {pTileX-1, pTileY}, {pTileX, pTileY+1}, {pTileX, pTileY-1}};
    for (int i = 0; i < 4; ++i) {
        int ax = adj[i][0];
        int ay = adj[i][1];
        if (g_map.get(ax, ay, 0) == 3 && g_player.getKeys() > 0) {
            g_player.setKeys(g_player.getKeys() - 1);
            g_map.set(ax, ay, 2);
            emitParticles((ax + 0.5f) * TILE_SIZE, (ay + 0.5f) * TILE_SIZE, rgb(255, 215, 0), 16);
            addFloater((ax + 0.5f) * TILE_SIZE, (ay + 0.5f) * TILE_SIZE, "UNLOCKED", rgb(255, 215, 0));
        }
    }

    int curTile = g_map.get(pTileX, pTileY, 0);
    if (curTile == 4) {
        g_player.setKeys(g_player.getKeys() + 1);
        g_map.set(pTileX, pTileY, 2);
        emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(255, 215, 0), 14);
        addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "+1 KEY", rgb(255, 215, 0));
    } else if (curTile == 5) {
        g_player.setGold(g_player.getGold() + 100);
        g_player.setScore(g_player.getScore() + 250);
        g_map.set(pTileX, pTileY, 2);
        emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(255, 215, 0), 20);
        addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "+100 G", rgb(255, 215, 0));
    } else if (curTile == 6) {
        g_player.setGold(g_player.getGold() + 35);
        g_player.setScore(g_player.getScore() + 80);
        g_map.set(pTileX, pTileY, 2);
        emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(255, 60, 100), 10);
        addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "+35 G", rgb(255, 100, 150));
    } else if (curTile == 7) {
        if (g_player.getHp() < g_player.getMaxHp()) {
            g_player.setHp(std::min(g_player.getMaxHp(), g_player.getHp() + 35));
            g_map.set(pTileX, pTileY, 2);
            emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(255, 40, 60), 12);
            addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "+35 HP", rgb(0, 255, 120));
        }
    } else if (curTile == 11) {
        g_player.setHasWand(true);
        g_player.setMana(g_player.getMaxMana());
        g_map.set(pTileX, pTileY, 2);
        emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(255, 120, 20), 20);
        addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "FIRE WAND! [F]", rgb(255, 140, 30));
    } else if (curTile == 12) {
        g_player.setHasBoots(true);
        g_map.set(pTileX, pTileY, 2);
        emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(80, 200, 255), 20);
        addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "DASH BOOTS! [SHIFT]", rgb(80, 220, 255));
    } else if (curTile == 8) {
        if (g_player.getInvulnTimer() <= 0.0f) {
            int newHp = g_player.getHp() - 15;
            g_player.setHp(newHp);
            g_player.setInvulnTimer(0.5f);
            g_shakeTimer = 0.2f;
            emitParticles((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE, rgb(200, 30, 40), 8);
            addFloater((pTileX + 0.5f) * TILE_SIZE, (pTileY + 0.5f) * TILE_SIZE - 10, "-15 HP", rgb(255, 50, 50));
            if (newHp <= 0) {
                g_player.setHp(0);
                g_fsm.transition(GameFsmEvent::Die);
                return;
            }
        }
    } else if (curTile == 9) {
        g_fsm.transition(GameFsmEvent::Win);
        return;
    }

    for (auto& e : g_enemies) {
        if (!e.getAlive()) continue;
        if (e.getHitTimer() > 0.0f) e.setHitTimer(e.getHitTimer() - 0.016f);
        if (e.getAttackCooldown() > 0.0f) e.setAttackCooldown(e.getAttackCooldown() - 0.016f);

        float dist = std::hypot(curX - e.getX(), curY - e.getY());
        float aggroDist = (e.getTypeId() == 2 ? 8.5f : 6.0f);

        if (dist < aggroDist) {
            float edx = (curX - e.getX()) / dist;
            float edy = (curY - e.getY()) / dist;
            float eNextX = e.getX() + edx * e.getSpeed();
            float eNextY = e.getY() + edy * e.getSpeed();

            if (!isSolid(static_cast<int>(eNextX), static_cast<int>(e.getY()))) e.setX(eNextX);
            if (!isSolid(static_cast<int>(e.getX()), static_cast<int>(eNextY))) e.setY(eNextY);

            if (dist < 0.65f && e.getAttackCooldown() <= 0.0f) {
                e.setAttackCooldown(0.70f);
                if (g_player.getInvulnTimer() <= 0.0f) {
                    int dmg = (e.getTypeId() == 2 ? 24 : 12);
                    int newHp = g_player.getHp() - dmg;
                    g_player.setHp(newHp);
                    g_player.setInvulnTimer(0.40f);
                    g_shakeTimer = 0.25f;
                    emitParticles(curX * TILE_SIZE, curY * TILE_SIZE, rgb(255, 40, 40), 10);
                    addFloater(curX * TILE_SIZE, curY * TILE_SIZE - 15, "-" + std::to_string(dmg), rgb(255, 30, 40));

                    if (newHp <= 0) {
                        g_player.setHp(0);
                        g_fsm.transition(GameFsmEvent::Die);
                        return;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < g_particles.size();) {
        auto& p = g_particles[i];
        p.setX(p.getX() + p.getVx());
        p.setY(p.getY() + p.getVy());
        p.setLife(p.getLife() - 0.016f);
        if (p.getLife() <= 0.0f) {
            g_particles.erase(g_particles.begin() + i);
        } else {
            ++i;
        }
    }

    for (size_t i = 0; i < g_floaters.size();) {
        g_floaters[i].y -= 0.65f;
        g_floaters[i].life -= 0.016f;
        if (g_floaters[i].life <= 0.0f) {
            g_floaters.erase(g_floaters.begin() + i);
        } else {
            ++i;
        }
    }

    clearScreen(rgb(10, 10, 16));

    float camX = curX * TILE_SIZE - SCREEN_W / 2.0f + g_shakeX;
    float camY = curY * TILE_SIZE - (SCREEN_H - HUD_H) / 2.0f + g_shakeY;
    camX = std::max(0.0f, std::min(camX, static_cast<float>(MAP_W * TILE_SIZE - SCREEN_W)));
    camY = std::max(0.0f, std::min(camY, static_cast<float>(MAP_H * TILE_SIZE - (SCREEN_H - HUD_H))));

    int startTileX = std::max(0, static_cast<int>(camX / TILE_SIZE));
    int endTileX = std::min(MAP_W, static_cast<int>((camX + SCREEN_W) / TILE_SIZE) + 1);
    int startTileY = std::max(0, static_cast<int>(camY / TILE_SIZE));
    int endTileY = std::min(MAP_H, static_cast<int>((camY + (SCREEN_H - HUD_H)) / TILE_SIZE) + 1);

    float pxPx = curX * TILE_SIZE;
    float pyPx = curY * TILE_SIZE;
    float torchFlk = 1.0f + 0.06f * std::sin(g_gameTime * 15.0f) + 0.03f * std::cos(g_gameTime * 23.0f);
    float playerTorchRadius = 180.0f * torchFlk;

    for (int ty = startTileY; ty < endTileY; ++ty) {
        for (int tx = startTileX; tx < endTileX; ++tx) {
            int screenX = static_cast<int>(tx * TILE_SIZE - camX);
            int screenY = static_cast<int>(ty * TILE_SIZE - camY) + HUD_H;
            if (screenX < -TILE_SIZE || screenX >= SCREEN_W || screenY < HUD_H - TILE_SIZE || screenY >= SCREEN_H) continue;

            float tileCenterPxX = (tx + 0.5f) * TILE_SIZE;
            float tileCenterPxY = (ty + 0.5f) * TILE_SIZE;

            float lr = 0.14f;
            float lg = 0.14f;
            float lb = 0.20f;

            float pDist = std::hypot(tileCenterPxX - pxPx, tileCenterPxY - pyPx);
            if (pDist < playerTorchRadius) {
                float atten = std::max(0.0f, 1.0f - (pDist / playerTorchRadius));
                atten = atten * atten;
                lr += atten * 1.0f;
                lg += atten * 0.82f;
                lb += atten * 0.55f;
            }

            for (const auto& torch : g_torches) {
                float tDist = std::hypot(tileCenterPxX - torch.x, tileCenterPxY - torch.y);
                if (tDist < torch.radius) {
                    float atten = std::max(0.0f, 1.0f - (tDist / torch.radius));
                    atten = atten * atten;
                    lr += atten * torch.r;
                    lg += atten * torch.g;
                    lb += atten * torch.b;
                }
            }

            for (const auto& pr : g_projectiles) {
                float prDist = std::hypot(tileCenterPxX - pr.getX() * TILE_SIZE, tileCenterPxY - pr.getY() * TILE_SIZE);
                if (prDist < 120.0f) {
                    float atten = std::max(0.0f, 1.0f - (prDist / 120.0f));
                    lr += atten * 1.0f;
                    lg += atten * 0.6f;
                    lb += atten * 0.1f;
                }
            }

            float portalDist = std::hypot(tileCenterPxX - 28.5f * TILE_SIZE, tileCenterPxY - 19.5f * TILE_SIZE);
            if (portalDist < 160.0f) {
                float atten = std::max(0.0f, 1.0f - (portalDist / 160.0f));
                atten = atten * atten;
                lr += atten * 0.4f;
                lg += atten * 0.9f;
                lb += atten * 1.1f;
            }

            lr = std::min(1.5f, lr);
            lg = std::min(1.5f, lg);
            lb = std::min(1.5f, lb);

            int tType = g_map.get(tx, ty, 1);
            if (tType == 1) drawStoneWall(screenX, screenY, lr, lg, lb);
            else if (tType == 2) drawFloor(screenX, screenY, lr, lg, lb);
            else if (tType == 3) drawDoor(screenX, screenY, lr, lg, lb);
            else if (tType == 4) drawKeyItem(screenX, screenY, lr, lg, lb);
            else if (tType == 5) drawChest(screenX, screenY, lr, lg, lb);
            else if (tType == 6) drawGem(screenX, screenY, lr, lg, lb);
            else if (tType == 7) drawPotion(screenX, screenY, lr, lg, lb);
            else if (tType == 8) drawSpikes(screenX, screenY, lr, lg, lb);
            else if (tType == 9) drawPortal(screenX, screenY);
            else if (tType == 10) drawTorchTile(screenX, screenY, lr, lg, lb);
            else if (tType == 11) drawWandItem(screenX, screenY, lr, lg, lb);
            else if (tType == 12) drawBootsItem(screenX, screenY, lr, lg, lb);
            else if (tType == 13) drawPotItem(screenX, screenY, lr, lg, lb);
        }
    }

    for (const auto& p : g_particles) {
        int sx = static_cast<int>(p.getX() - camX);
        int sy = static_cast<int>(p.getY() - camY) + HUD_H;
        if (sx >= 0 && sx < SCREEN_W && sy >= HUD_H && sy < SCREEN_H) {
            int sz = static_cast<int>(p.getSize());
            drawRect(sx, sy, sz, sz, p.getColor());
        }
    }

    for (const auto& pr : g_projectiles) {
        int sx = static_cast<int>(pr.getX() * TILE_SIZE - camX);
        int sy = static_cast<int>(pr.getY() * TILE_SIZE - camY) + HUD_H;
        if (sx >= 0 && sx < SCREEN_W && sy >= HUD_H && sy < SCREEN_H) {
            drawRect(sx - 2, sy - 2, 7, 7, rgb(255, 100, 20));
            drawRect(sx - 1, sy - 1, 5, 5, rgb(255, 230, 60));
            setPixel(sx + 1, sy + 1, rgb(255, 255, 255));
        }
    }

    for (const auto& e : g_enemies) {
        if (!e.getAlive()) continue;
        int sx = static_cast<int>(e.getX() * TILE_SIZE - camX);
        int sy = static_cast<int>(e.getY() * TILE_SIZE - camY) + HUD_H;
        if (sx >= -TILE_SIZE && sx < SCREEN_W && sy >= HUD_H - TILE_SIZE && sy < SCREEN_H) {
            drawEnemySprite(e, sx, sy);
        }
    }

    int playerSx = static_cast<int>(curX * TILE_SIZE - camX);
    int playerSy = static_cast<int>(curY * TILE_SIZE - camY) + HUD_H;
    drawPlayerSprite(playerSx, playerSy, g_player.getFacing(), g_player.getInvulnTimer() > 0.0f);

    if (g_slashTimer > 0.0f) {
        int slSx = static_cast<int>(g_slashX * TILE_SIZE - camX);
        int slSy = static_cast<int>(g_slashY * TILE_SIZE - camY) + HUD_H;
        uint32_t slashColor = rgb(180, 235, 255);
        if (g_slashDir == 0) {
            drawRect(slSx - 12, slSy + 4, 30, 4, slashColor);
            drawRect(slSx - 8, slSy + 8, 22, 2, rgb(255, 255, 255));
        } else if (g_slashDir == 1) {
            drawRect(slSx - 12, slSy - 4, 30, 4, slashColor);
            drawRect(slSx - 8, slSy - 6, 22, 2, rgb(255, 255, 255));
        } else if (g_slashDir == 2) {
            drawRect(slSx - 4, slSy - 12, 4, 30, slashColor);
            drawRect(slSx - 6, slSy - 8, 2, 22, rgb(255, 255, 255));
        } else if (g_slashDir == 3) {
            drawRect(slSx + 4, slSy - 12, 4, 30, slashColor);
            drawRect(slSx + 6, slSy - 8, 2, 22, rgb(255, 255, 255));
        }
    }

    for (const auto& flt : g_floaters) {
        int fx = static_cast<int>(flt.x - camX);
        int fy = static_cast<int>(flt.y - camY) + HUD_H;
        if (fx >= 0 && fx < SCREEN_W - 60 && fy >= HUD_H && fy < SCREEN_H) {
            drawString(fx, fy, flt.text, flt.color, 1, true);
        }
    }

    drawRect(0, 0, SCREEN_W, HUD_H, rgb(15, 18, 28));
    drawRect(0, HUD_H - 2, SCREEN_W, 2, rgb(215, 175, 40));

    drawString(15, 14, "HP", rgb(255, 80, 80), 1);
    drawRect(35, 13, 110, 16, rgb(40, 15, 20));
    float hpPercent = std::max(0.0f, std::min(1.0f, static_cast<float>(g_player.getHp()) / static_cast<float>(g_player.getMaxHp())));
    drawRect(37, 15, static_cast<int>(106.0f * hpPercent), 12, rgb(235, 30, 45));
    drawRect(37, 15, static_cast<int>(106.0f * hpPercent), 3, rgb(255, 120, 130));
    drawString(65, 16, std::to_string(std::max(0, g_player.getHp())) + "/" + std::to_string(g_player.getMaxHp()), rgb(255, 255, 255), 1);

    drawString(160, 14, "MP", rgb(60, 160, 255), 1);
    drawRect(180, 13, 80, 16, rgb(15, 25, 45));
    float mpPercent = std::max(0.0f, std::min(1.0f, static_cast<float>(g_player.getMana()) / static_cast<float>(g_player.getMaxMana())));
    drawRect(182, 15, static_cast<int>(76.0f * mpPercent), 12, rgb(30, 140, 255));
    drawRect(182, 15, static_cast<int>(76.0f * mpPercent), 3, rgb(120, 200, 255));
    drawString(205, 16, std::to_string(g_player.getMana()), rgb(255, 255, 255), 1);

    drawRect(280, 17, 8, 8, rgb(255, 215, 0));
    drawString(295, 16, std::to_string(g_player.getGold()), rgb(255, 215, 0), 1);
    drawString(360, 16, "PTS: " + std::to_string(g_player.getScore()), rgb(180, 220, 255), 1);

    uint32_t keyCol = (g_player.getKeys() > 0) ? rgb(0, 255, 120) : rgb(140, 150, 160);
    drawString(460, 16, "[KEY: " + std::to_string(g_player.getKeys()) + "]", keyCol, 1);

    if (g_player.getHasWand()) {
        drawString(540, 16, "[FIRE WAND: F]", rgb(255, 140, 30), 1);
    }
    if (g_player.getHasBoots()) {
        drawString(650, 16, "[BOOTS: SHIFT]", rgb(80, 200, 255), 1);
    }

    int mapBoxX = SCREEN_W - 138;
    int mapBoxY = HUD_H + 10;
    int mapBoxW = MAP_W * 4 + 4;
    int mapBoxH = MAP_H * 4 + 4;

    drawRect(mapBoxX - 2, mapBoxY - 2, mapBoxW + 4, mapBoxH + 4, rgb(215, 175, 40));
    drawRect(mapBoxX, mapBoxY, mapBoxW, mapBoxH, rgb(10, 12, 18));

    for (int my = 0; my < MAP_H; ++my) {
        for (int mx = 0; mx < MAP_W; ++mx) {
            int t = g_map.get(mx, my, 1);
            uint32_t c = rgb(25, 28, 40);
            if (t == 1 || t == 10) c = rgb(65, 70, 90);
            else if (t == 2) c = rgb(20, 22, 32);
            else if (t == 3) c = rgb(255, 180, 0);
            else if (t == 4) c = rgb(255, 230, 50);
            else if (t == 5 || t == 6) c = rgb(255, 215, 0);
            else if (t == 9) c = rgb(0, 255, 255);
            else if (t == 11) c = rgb(255, 120, 20);
            else if (t == 12) c = rgb(80, 200, 255);
            drawRect(mapBoxX + 2 + mx * 4, mapBoxY + 2 + my * 4, 3, 3, c);
        }
    }

    for (const auto& e : g_enemies) {
        if (!e.getAlive()) continue;
        int emx = mapBoxX + 2 + static_cast<int>(e.getX() * 4);
        int emy = mapBoxY + 2 + static_cast<int>(e.getY() * 4);
        drawRect(emx, emy, 3, 3, rgb(255, 30, 40));
    }

    int pmx = mapBoxX + 2 + static_cast<int>(curX * 4);
    int pmy = mapBoxY + 2 + static_cast<int>(curY * 4);
    drawRect(pmx, pmy, 4, 4, rgb(0, 240, 255));
}

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#endif

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

static uint32_t s_pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
static bool s_keys[256] = { false };
static bool s_running = true;

inline void setPixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        s_pixels[y * SCREEN_WIDTH + x] = color;
    }
}

inline void clearScreen(uint32_t color) {
    std::fill(std::begin(s_pixels), std::end(s_pixels), color);
}

// rysuje prostokąt piksel po pikselu
inline void drawRect(int x, int y, int w, int h, uint32_t color) {
    for (int dy = 0; dy < h; ++dy) {
        int py = y + dy;
        if (py < 0 || py >= SCREEN_HEIGHT) continue;
        for (int dx = 0; dx < w; ++dx) {
            int px = x + dx;
            if (px >= 0 && px < SCREEN_WIDTH) {
                s_pixels[py * SCREEN_WIDTH + px] = color;
            }
        }
    }
}

inline bool isKeyDown(int key) {
    if (key >= 0 && key < 256) return s_keys[key];
    return false;
}

inline bool isKeyPressed(std::string_view key) {
    if (key == "w" || key == "W" || key == "up") return s_keys['W'] || s_keys[38];
    if (key == "s" || key == "S" || key == "down") return s_keys['S'] || s_keys[40];
    if (key == "a" || key == "A" || key == "left") return s_keys['A'] || s_keys[37];
    if (key == "d" || key == "D" || key == "right") return s_keys['D'] || s_keys[39];
    if (key == "space" || key == " " || key == "shoot") return s_keys[' '] || s_keys[13];
    if (key == "q" || key == "Q" || key == "escape") return s_keys['Q'] || s_keys[27];
    return false;
}

#if defined(_WIN32)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY:
        s_running = false;
        PostQuitMessage(0);
        return 0;
        case WM_KEYDOWN:
        if (wParam < 256) s_keys[wParam] = true;
        if (wParam == VK_ESCAPE) s_running = false;
        return 0;
        case WM_KEYUP:
        if (wParam < 256) s_keys[wParam] = false;
        return 0;
        default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
#endif

// tworzy okno i uruchamia kolejne klatki gry
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    #if defined(_WIN32)
    const char CLASS_NAME[] = "McrGameWindowClass";
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassEx(&wc);

    RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
    0, CLASS_NAME, "Dungeon of the Forgotten Crypt",
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
    nullptr, nullptr, GetModuleHandle(nullptr), nullptr
    );

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = SCREEN_WIDTH;
    bmi.bmiHeader.biHeight = -SCREEN_HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    MSG msg;
    while (s_running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) s_running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        updateGame();
        HDC hdc = GetDC(hwnd);
        StretchDIBits(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, s_pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);
        ReleaseDC(hwnd, hdc);
        Sleep(16);
    }
    #else
    for (int frame = 0; frame < 100 && s_running; ++frame) {
        updateGame();
    }
    #endif
    return 0;
}
