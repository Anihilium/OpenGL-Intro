#include <Resources/Model.hpp>

namespace Resources
{
	Model::Model(const char* path)
	{
		LoadData(path);
	}

	Model::~Model()
	{
		mVertexBuffer.clear();
		mIndexBuffer.clear();
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}

	void Model::LoadData(const char* path)
	{
		FILE* file;

		char binName[128];
		int nameIterator = 0;
		for (nameIterator = 0; path[nameIterator] != '.'; ++nameIterator)
			binName[nameIterator] = path[nameIterator];
		binName[nameIterator] = '.'; binName[nameIterator + 1] = 'b'; binName[nameIterator + 2] = 'i'; binName[nameIterator + 3] = 'n'; binName[nameIterator + 4] = '\0';
		
		fopen_s(&file, binName, "rb");
		if (file != nullptr)
		{
			int vertexAmount, indexAmount;
			fread_s(&vertexAmount, sizeof(int), sizeof(int), 1, file);
			fread_s(&indexAmount, sizeof(int), sizeof(int), 1, file);

			for (int i = 0; i < vertexAmount; i++)
			{
				Vertex vtx;
				fread_s(&vtx, sizeof(Vertex), sizeof(Vertex), 1, file);
				mVertexBuffer.push_back(vtx);
			}

			for (int i = 0; i < indexAmount; i++)
			{
				uint32_t index;
				fread_s(&index, sizeof(uint32_t), sizeof(uint32_t), 1, file);
				mIndexBuffer.push_back(index);
			}

			fclose(file);
			SetBuffers();
			return;
		}

		fopen_s(&file, path, "r");
		ASSERT(file != nullptr, "Could not open file")

		std::vector<Vec3> vertexPositions;
		std::vector<Vec2> vertexTextureCoord;
		std::vector<Vec3> vertexNormal;

		char header;
		Vec3 vPos;
		Vec2 vTex;
		Vec3 vNorm;

		do
		{
			header = fgetc(file);
			if (header == 'v')
			{
				header = fgetc(file);
				switch (header)
				{
				case ' ':
					if (!fscanf_s(file, "%f %f %f\n", &vPos.x, &vPos.y, &vPos.z))
						return;
					vertexPositions.push_back(vPos);
					break;
				case 't':
					if(!fscanf_s(file, " %f %f\n", &vTex.x, &vTex.y))
						return;
					vertexTextureCoord.push_back(vTex);
					break;
				case 'n':
					if(!fscanf_s(file, " %f %f %f\n", &vNorm.x, &vNorm.y, &vNorm.z))
						return;
					vertexNormal.push_back(vNorm);
					break;
				default:
					break;
				}
			}
			else if (header == 'f')
			{
				header = fgetc(file);
				while (header != '\n' && header != EOF)
				{
					int vtxData[3];
					if(!fscanf_s(file, "%d/%d/%d", &vtxData[0], &vtxData[1], &vtxData[2]))
						return;

					Vertex vtx = { vertexPositions[vtxData[0] - 1], vertexNormal[vtxData[2] - 1], vertexTextureCoord[vtxData[1] - 1] };

					uint32_t j = 0;
					bool inVector = false;
					for(Vertex& v : mVertexBuffer)
					{
						if (v.Position == vtx.Position && v.TextureUV == vtx.TextureUV && v.Normal == vtx.Normal)
						{
							mIndexBuffer.push_back(j);
							inVector = true;
							break;
						}
						j++;
					}

					if (!inVector)
					{
						mIndexBuffer.push_back(mVertexBuffer.size());
						mVertexBuffer.push_back(vtx);
					}
					header = fgetc(file);
				}
			}
		} while (header != EOF);

		fclose(file);
		
		SetBuffers();

		fopen_s(&file, binName, "wb");
		if (file != nullptr)
		{
			int vertexSize = mVertexBuffer.size(), indexSize = mIndexBuffer.size();
			fwrite(&vertexSize, sizeof(int), 1, file);
			fwrite(&indexSize, sizeof(int), 1, file);
			for(Vertex& vtx: mVertexBuffer)
				fwrite(&vtx, sizeof(Vertex), 1, file);
			for(uint32_t& indx: mIndexBuffer)
				fwrite(&indx, sizeof(uint32_t), 1, file);
		
			fclose(file);
		}
	}

	void Model::SetBuffers()
	{
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------

		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mEBO);

		glGenVertexArrays(1, &VAO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexBuffer.size() * sizeof(Resources::Vertex), mVertexBuffer.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer.size() * sizeof(uint32_t), mIndexBuffer.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)sizeof(Vec3));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (void*)(2 * sizeof(Vec3)));
		glEnableVertexAttribArray(2);
	}
}