//==============================================================================
// xxGraphic : Material Header
//
// Copyright (c) 2019-2025 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#pragma once

#include "xxUtility.h"

class xxPlusAPI xxMaterial
{
public:
    virtual void                Invalidate();
    virtual void                Update(float time);
    virtual void                Setup(xxDrawData const& data);
    virtual void                Draw(xxDrawData const& data) const;

    xxTexturePtr const&         GetTexture(size_t index) const;
    void                        SetTexture(size_t index, xxTexturePtr const& texture);

    virtual void                CreatePipeline(xxDrawData const& data);
    virtual void                CreateConstant(xxDrawData const& data) const;
    virtual void                UpdateConstant(xxDrawData const& data) const;

    static xxMaterialPtr        Create();

    static xxMaterialPtr      (*BinaryCreate)();
    virtual void                BinaryRead(xxBinary& binary);
    virtual void                BinaryWrite(xxBinary& binary) const;

protected:
    xxMaterial();
    virtual ~xxMaterial();

    virtual std::string         GetShader(xxDrawData const& data, int type) const;
    virtual int                 GetVertexConstantSize(xxDrawData const& data) const;
    virtual int                 GetFragmentConstantSize(xxDrawData const& data) const;

    uint64_t                    m_device = 0;
    uint64_t                    m_blendState = 0;
    uint64_t                    m_depthStencilState = 0;
    uint64_t                    m_rasterizerState = 0;
    uint64_t                    m_vertexShader = 0;
    uint64_t                    m_fragmentShader = 0;
    uint64_t                    m_renderPass = 0;
    uint64_t                    m_pipeline = 0;

public:
    std::string                 Name = "";

    std::string                 Shader = "";
    std::string                 ShaderOption = "";

    xxVector3                   AmbientColor = xxVector3::ZERO;
    xxVector3                   DiffuseColor = xxVector3::ZERO;
    xxVector3                   EmissiveColor = xxVector3::ZERO;
    xxVector3                   SpecularColor = xxVector3::ZERO;
    float                       SpecularHighlight = 0.0f;
    float                       Opacity = 1.0f;

    bool                        Lighting = false;
    bool                        Specular = false;

    bool                        AlphaTest = false;
    float                       AlphaTestReference = 0.5f;

    bool                        Blending = false;
    std::string                 BlendSourceColor = "SrcAlpha";
    std::string                 BlendOperationColor = "";
    std::string                 BlendDestinationColor = "1-SrcAlpha";
    std::string                 BlendSourceAlpha = "1";
    std::string                 BlendOperationAlpha = "";
    std::string                 BlendDestinationAlpha = "0";

    std::string                 DepthTest = "";
    bool                        DepthWrite = false;

    bool                        Cull = false;
    bool                        Scissor = false;

    std::vector<xxVector4>      Colors;
    std::vector<xxTexturePtr>   Textures;

    std::vector<xxModifierData> Modifiers;

    static char const           DefaultShader[];
};
