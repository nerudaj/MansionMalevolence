#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace tiled
{

    // docs: https://doc.mapeditor.org/en/stable/reference/json-map-format/

    enum class [[nodiscard]] Orientation
    {
        Orthogonal,
        Isometric,
        Staggered,
        Hexagonal,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        Orientation,
        {
            { Orientation::Orthogonal, "orthogonal" },
            { Orientation::Isometric, "isometric" },
            { Orientation::Staggered, "staggered" },
            { Orientation::Hexagonal, "hexagonal" },
        });

    enum class [[nodiscard]] RenderOrder
    {
        RightDown,
        RightUp,
        LeftDown,
        LeftUp,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        RenderOrder,
        {
            { RenderOrder::RightDown, "right-down" },
            { RenderOrder::RightUp, "right-up" },
            { RenderOrder::LeftDown, "left-down" },
            { RenderOrder::LeftUp, "left-up" },
        });

    enum class [[nodiscard]] DrawOrder
    {
        TopDown,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        DrawOrder, { { DrawOrder::TopDown, "topdown" } });

    enum class [[nodiscard]] LayerType
    {
        TileLayer,
        ObjectGroup,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        LayerType,
        {
            { LayerType::TileLayer, "tilelayer" },
            { LayerType::ObjectGroup, "objectgroup" },
        });

    enum class [[nodiscard]] MapType
    {
        Map,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        MapType,
        {
            { MapType::Map, "map" },
        });

    enum class [[nodiscard]] StaggerAxis
    {
        X,
        Y,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        StaggerAxis,
        {
            { StaggerAxis::X, "x" },
            { StaggerAxis::Y, "y" },
        });

    enum class [[nodiscard]] StaggerIndex
    {
        Odd,
        Even,
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        StaggerIndex,
        {
            { StaggerIndex::Odd, "odd" },
            { StaggerIndex::Even, "even" },
        });

    struct TileLayerModel
    {
        unsigned id = 0;
        std::string name = "";
        LayerType type;
        std::vector<int> data = {};
        int x = 0;
        int y = 0;
        unsigned width = 0;
        unsigned height = 0;
        unsigned opacity = 0;
        bool visible = true;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        TileLayerModel,
        id,
        name,
        data,
        x,
        y,
        width,
        height,
        opacity,
        type,
        visible);

    struct TilesetModel
    {
        unsigned firstgid = 0;
        std::string source = "";
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TilesetModel, firstgid, source);

    struct ObjectModel
    {
        int id = 0;
        float x = 0;
        float y = 0;
        unsigned width = 0;
        unsigned height = 0;
        std::string name;
        std::string type;
        int rotation = 0;
        bool point = true;
        bool visible = true;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        ObjectModel,
        id,
        x,
        y,
        width,
        height,
        name,
        type,
        rotation,
        point,
        visible);

    struct ObjectGroupModel
    {
        int id = 0;
        int x = 0;
        int y = 0;
        std::string name;
        std::vector<ObjectModel> objects = {};
        DrawOrder draworder;
        LayerType type;
        bool visible = true;
        unsigned opacity = 0;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        ObjectGroupModel,
        id,
        x,
        y,
        name,
        objects,
        draworder,
        type,
        visible,
        opacity);

    using LayerModel = std::variant<TileLayerModel, ObjectGroupModel>;

    struct FiniteMapModel
    {
        int compressionlevel = 0;
        int nextlayerid = 0;
        int nextobjectid = 0;
        unsigned width = 0;
        unsigned height = 0;
        unsigned tilewidth = 0;
        unsigned tileheight = 0;
        std::vector<LayerModel> layers;
        bool infinite = false;
        Orientation orientation = {};
        RenderOrder renderorder = {};
        std::vector<TilesetModel> tilesets = {};
        MapType type = {};
        std::string tiledversion = "";
        std::string version = "";
        double parallaxoriginx = 0.0;
        double parallaxoriginy = 0.0;
        StaggerAxis staggeraxis = {};
        StaggerIndex staggerindex = {};
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        FiniteMapModel,
        compressionlevel,
        nextlayerid,
        nextobjectid,
        width,
        height,
        tilewidth,
        tileheight,
        layers,
        infinite,
        orientation,
        renderorder,
        tilesets,
        type,
        tiledversion,
        version);

} // namespace tiled

namespace nlohmann
{
    template<>
    struct adl_serializer<tiled::LayerModel>
    {
        static void to_json(json& j, const tiled::LayerModel& opt)
        {
            if (std::holds_alternative<tiled::TileLayerModel>(opt))
            {
                j = std::get<tiled::TileLayerModel>(opt);
            }
            else
            {
                j = std::get<tiled::ObjectGroupModel>(opt);
            }
        }

        static void from_json(const json& j, tiled::LayerModel& opt)
        {
            if (j.contains("type"))
            {
                tiled::LayerType type;
                j["type"].get_to(type);

                if (type == tiled::LayerType::TileLayer)
                {
                    tiled::TileLayerModel model = j;
                    opt = model;
                }
                else
                {
                    tiled::ObjectGroupModel model = j;
                    opt = model;
                }
            }
        }
    };
} // namespace nlohmann