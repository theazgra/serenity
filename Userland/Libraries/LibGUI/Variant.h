/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <AK/String.h>
#include <LibGUI/Icon.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Font.h>

namespace GUI {

class Variant {
public:
    Variant();
    Variant(bool);
    Variant(float);
    Variant(i32);
    Variant(i64);
    Variant(unsigned);
    Variant(const char*);
    Variant(const String&);
    Variant(const FlyString&);
    Variant(const Gfx::Bitmap&);
    Variant(const GUI::Icon&);
    Variant(const Gfx::IntPoint&);
    Variant(const Gfx::IntSize&);
    Variant(const Gfx::IntRect&);
    Variant(const Gfx::Font&);
    Variant(const Gfx::TextAlignment);
    Variant(const AK::JsonValue&);
    Variant(Color);

    Variant(const Variant&);
    Variant& operator=(const Variant&);

    Variant(Variant&&) = delete;
    Variant& operator=(Variant&&);

    void clear();
    ~Variant();

    enum class Type {
        Invalid,
        Bool,
        Int32,
        Int64,
        UnsignedInt,
        Float,
        String,
        Bitmap,
        Color,
        Icon,
        Point,
        Size,
        Rect,
        Font,
        TextAlignment,
    };

    bool is_valid() const { return m_type != Type::Invalid; }
    bool is_bool() const { return m_type == Type::Bool; }
    bool is_i32() const { return m_type == Type::Int32; }
    bool is_i64() const { return m_type == Type::Int64; }
    bool is_uint() const { return m_type == Type::UnsignedInt; }
    bool is_float() const { return m_type == Type::Float; }
    bool is_string() const { return m_type == Type::String; }
    bool is_bitmap() const { return m_type == Type::Bitmap; }
    bool is_color() const { return m_type == Type::Color; }
    bool is_icon() const { return m_type == Type::Icon; }
    bool is_point() const { return m_type == Type::Point; }
    bool is_size() const { return m_type == Type::Size; }
    bool is_rect() const { return m_type == Type::Rect; }
    bool is_font() const { return m_type == Type::Font; }
    bool is_text_alignment() const { return m_type == Type::TextAlignment; }
    Type type() const { return m_type; }

    bool as_bool() const
    {
        VERIFY(type() == Type::Bool);
        return m_value.as_bool;
    }

    bool to_bool() const
    {
        if (type() == Type::Bool)
            return as_bool();
        if (type() == Type::String)
            return !!m_value.as_string;
        if (type() == Type::Int32)
            return m_value.as_i32 != 0;
        if (type() == Type::Int64)
            return m_value.as_i64 != 0;
        if (type() == Type::UnsignedInt)
            return m_value.as_uint != 0;
        if (type() == Type::Rect)
            return !as_rect().is_null();
        if (type() == Type::Size)
            return !as_size().is_null();
        if (type() == Type::Point)
            return !as_point().is_null();
        return is_valid();
    }

    int as_i32() const
    {
        VERIFY(type() == Type::Int32);
        return m_value.as_i32;
    }

    int as_i64() const
    {
        VERIFY(type() == Type::Int64);
        return m_value.as_i64;
    }

    unsigned as_uint() const
    {
        VERIFY(type() == Type::UnsignedInt);
        return m_value.as_uint;
    }

    template<typename T>
    T to_integer() const
    {
        if (is_i32())
            return as_i32();
        if (is_i64())
            return as_i64();
        if (is_bool())
            return as_bool() ? 1 : 0;
        if (is_float())
            return (int)as_float();
        if (is_uint()) {
            VERIFY(as_uint() <= INT32_MAX);
            return (int)as_uint();
        }
        if (is_string())
            return as_string().to_int().value_or(0);
        return 0;
    }

    i32 to_i32() const
    {
        return to_integer<i32>();
    }

    i64 to_i64() const
    {
        return to_integer<i64>();
    }

    float as_float() const
    {
        VERIFY(type() == Type::Float);
        return m_value.as_float;
    }

    Gfx::IntPoint as_point() const
    {
        return { m_value.as_point.x, m_value.as_point.y };
    }

    Gfx::IntSize as_size() const
    {
        return { m_value.as_size.width, m_value.as_size.height };
    }

    Gfx::IntRect as_rect() const
    {
        return { as_point(), as_size() };
    }

    String as_string() const
    {
        VERIFY(type() == Type::String);
        return m_value.as_string;
    }

    const Gfx::Bitmap& as_bitmap() const
    {
        VERIFY(type() == Type::Bitmap);
        return *m_value.as_bitmap;
    }

    GUI::Icon as_icon() const
    {
        VERIFY(type() == Type::Icon);
        return GUI::Icon(*m_value.as_icon);
    }

    Color as_color() const
    {
        VERIFY(type() == Type::Color);
        return Color::from_rgba(m_value.as_color);
    }

    const Gfx::Font& as_font() const
    {
        VERIFY(type() == Type::Font);
        return *m_value.as_font;
    }

    Gfx::TextAlignment to_text_alignment(Gfx::TextAlignment default_value) const
    {
        if (type() != Type::TextAlignment)
            return default_value;
        return m_value.as_text_alignment;
    }

    Color to_color(Color default_value = {}) const
    {
        if (type() == Type::Color)
            return as_color();
        if (type() == Type::String) {
            auto color = Color::from_string(as_string());
            if (color.has_value())
                return color.value();
        }
        return default_value;
    }

    String to_string() const;

    bool operator==(const Variant&) const;
    bool operator<(const Variant&) const;

private:
    void copy_from(const Variant&);
    void move_from(Variant&&);

    struct RawPoint {
        int x;
        int y;
    };

    struct RawSize {
        int width;
        int height;
    };

    struct RawRect {
        RawPoint location;
        RawSize size;
    };

    union {
        StringImpl* as_string;
        Gfx::Bitmap* as_bitmap;
        GUI::IconImpl* as_icon;
        Gfx::Font* as_font;
        bool as_bool;
        i32 as_i32;
        i64 as_i64;
        unsigned as_uint;
        float as_float;
        Gfx::RGBA32 as_color;
        Gfx::TextAlignment as_text_alignment;
        RawPoint as_point;
        RawSize as_size;
        RawRect as_rect;
    } m_value;

    Type m_type { Type::Invalid };
};

const char* to_string(Variant::Type);

}
