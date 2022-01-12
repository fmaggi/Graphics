struct Mandelbrot : public Module
{
    ~Mandelbrot() override {}

    void OnAttach(uint32_t width, uint32_t height, EventSystem* eventSystem) override
    {
        m_width = width, m_height = height;
        camera.Move({-50, 0});

        shader = new Shader("mandelbrotV.glsl", "mandelbrotF.glsl");
        vao = new VertexArray;

        glm::vec2 vertices[] = {
            {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
        };

        vbo = new VertexBuffer(sizeof(glm::vec2), 4, VertexBuffer::BufferType::Static, vertices);

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        ibo = new IndexBuffer(6, indices);
        vbo->AddAttribute(2);

        eventSystem->RegisterListener<&Mandelbrot::OnMouseMoved>(this);
        eventSystem->RegisterListener<&Mandelbrot::OnMouseScrolled>(this);
        eventSystem->RegisterListener<&Mandelbrot::OnWindowResize>(this);
        eventSystem->RegisterListener<&Mandelbrot::OnKeyPressed>(this);
    }

    void OnRender() override
    {
        vao->Bind();
        ibo->Bind();

        shader->SetData<float>("width", m_width);
        shader->SetData<float>("height", m_height);
        shader->SetData<float>("zoom", camera.zoom);
        shader->SetData("pos", camera.translation);

        glm::vec2 base = Input::GetCursorPosition();
        shader->SetData("base", base);

        Renderer::DrawIndexed(vbo, shader, 6);
    }

    bool OnMouseMoved(MouseMoved event)
    {
        if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return false;
        camera.Move({-event.dx, -event.dy});
        return false;
    }

    bool OnMouseScrolled(MouseScrolled event)
    {
        camera.Zoom(event.dy);
        return false;
    }

    bool OnWindowResize(WindowResize event)
    {
        m_width = event.width;
        m_height = event.height;
        return false;
    }

    bool OnKeyPressed(KeyPressed event)
    {
        if (event.key == KEY_R)
        {
            delete shader;
            shader = new Shader("mandelbrotV.glsl", "mandelbrotF.glsl");
            return true;
        }
        return false;
    }

    void OnDetach(EventSystem* eventSystem) override
    {
        delete shader;
        delete vbo;
        delete vao;
        delete ibo;
    }

    Camera camera;
    Shader* shader;
    VertexBuffer* vbo;
    VertexArray* vao;
    IndexBuffer* ibo;
};
