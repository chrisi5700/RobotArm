#include <QtRendering/InstancedObject.hpp>

InstancedObject::InstancedObject(const std::vector<Vertex>& verts)
    : m_vertex_count(verts.size())
    , m_index_count(0)
{
    setup_vao(verts, {});
}

InstancedObject::InstancedObject(const MeshData& mesh_data)
    : m_vertex_count(mesh_data.vertices.size())
    , m_index_count(mesh_data.indices.size())
{
    setup_vao(mesh_data.vertices, mesh_data.indices);
}

void InstancedObject::setup_vao(const std::vector<Vertex>& verts, const std::vector<uint32_t>& indices)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vert_vbo);
    glGenBuffers(1, &m_instance_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    if (!indices.empty()) {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);

    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(2 + i, 1);
    }

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                          (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

InstancedObject::~InstancedObject()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vert_vbo);
    glDeleteBuffers(1, &m_instance_vbo);
    if (m_ebo) {
        glDeleteBuffers(1, &m_ebo);
    }
}

std::vector<InstanceData>& InstancedObject::get_instance_data()
{
    return m_instances;
}

void InstancedObject::draw(float)
{
    if (m_instances.empty()) return;

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_instances.size() * sizeof(InstanceData),
                 m_instances.data(), GL_DYNAMIC_DRAW);

    if (m_index_count > 0) {
        glDrawElementsInstanced(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0, m_instances.size());
    } else {
        glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertex_count, m_instances.size());
    }
}