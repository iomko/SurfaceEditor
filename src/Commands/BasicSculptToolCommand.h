#pragma once

class BasicSculptToolCommand : public Command<>
{
public:

    virtual void execute() override
    {

    }

    glm::vec3 computeAverageNormal(const std::vector<glm::vec3>& normals) {
        glm::vec3 sum(0.0f);
        float size = normals.size();
        for (const auto& normal : normals) {
            sum += normal;
        }

        glm::vec3 averageNormal = sum / size;

        return glm::normalize(averageNormal);
    }

    std::pair<bool, float> isPointInsideSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius) {
        float distance = glm::distance(point, sphereCenter);
        bool isInside = (distance <= sphereRadius);
        return std::make_pair(isInside, distance);
    }

    float logarithmicInterpolation(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
        return log(x * (exp(1) - 1) + 1) / log(exp(1));
    }

    float smoothstep(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
        return x * x * (3 - 2 * x);
        //return x * x * x * (x * (x * 6 - 15) + 10);
    }

    float calculateBrushScalingFactor(float distance, float radius, float brushStrength) {
        float t = 1.0f - (distance / radius);
        t = std::clamp(t, 0.0f, 1.0f);

        float scalingFactor = smoothstep(0.0f, 1.0f, t);
        scalingFactor *= brushStrength;

        return scalingFactor;
    }

    
    std::pair<std::vector<HalfEdgeDS::Vertex*>, std::vector<HalfEdgeDS::Face*>> accessNeighbouringVertsAndFacesOfVertex(HalfEdgeDS::Vertex* vertex)
    {
       
        HalfEdgeDS::HalfEdge* startingHalfEdge = vertex->m_halfEdge;
        HalfEdgeDS::HalfEdge* currentHalfEdge = startingHalfEdge;
        bool previouslyAccessedNext = true;
        bool previouslyVisitedBoundaryEdge = false;
        bool visitedTwoBoundaryEdges = false;

        std::vector<HalfEdgeDS::Face*> returnedFacesIndices;
        std::vector<HalfEdgeDS::Vertex*> returnedVerticesIndices;
        //ak sme predtym accessli Next tak to bude TRUE
        //ak sme predtym accessli Previous tak to bude FALSE

        do {
            returnedFacesIndices.push_back(currentHalfEdge->m_face);
            //returnedFacesIndices.push_back(currentHalfEdge->getFaceIndex());
            //returnedFaces.push_back(&(*currentHalfEdge->m_face));

            
            
			if (currentHalfEdge->m_vertex == vertex)
			{
                returnedVerticesIndices.push_back(currentHalfEdge->m_next->m_vertex);
				//returnedVerticesIndices.push_back((*currentHalfEdge->m_next).getVertexIndex());
				//returnedVertices.push_back(&(*(*currentHalfEdge->m_next).m_vertex));
			}
            else
            {
                returnedVerticesIndices.push_back(currentHalfEdge->m_vertex);
                //returnedVerticesIndices.push_back((currentHalfEdge->getVertexIndex()));
                //returnedVertices.push_back(&(*currentHalfEdge->m_vertex));
            }

            if(!currentHalfEdge->m_twin)
            {
				if (previouslyVisitedBoundaryEdge)
				{
					visitedTwoBoundaryEdges = true;
				}
				else
				{
					previouslyVisitedBoundaryEdge = true;

                    currentHalfEdge = startingHalfEdge->m_previous;
					//currentHalfEdge = &(*startingHalfEdge->m_previous);
					//musim taktiez nastavit ze som ziskal tento currentHalfEdge
					//pomocou Previousa
					previouslyAccessedNext = false;


                    returnedVerticesIndices.push_back(currentHalfEdge->m_vertex);
					//returnedVerticesIndices.push_back((currentHalfEdge->getVertexIndex()));
					//returnedVertices.push_back(&(*currentHalfEdge->m_vertex));

                   
                    if(currentHalfEdge->m_twin)
                    {
						//ak je v opacnom smere

                        if(currentHalfEdge->m_vertex != currentHalfEdge->m_twin->m_vertex)
                        {
                            currentHalfEdge = currentHalfEdge->m_twin->m_previous;
							//currentHalfEdge = &(*currentHalfEdge->m_twin->m_previous);
							//musim taktiez nastavit ze som ziskal tento currentHalfEdge
							//pomocou Previousa
							previouslyAccessedNext = false;
                        }
                        else
                        {
                            currentHalfEdge = currentHalfEdge->m_twin->m_next;
							//currentHalfEdge = &(*currentHalfEdge->m_twin->m_next);
							//musim taktiez nastavit ze som ziskal tento currentHalfEdge
							//pomocou Nexta
							previouslyAccessedNext = true;
                        }
                    }
					else
					{
						visitedTwoBoundaryEdges = true;
						//skoncil algoritmus
					}
				}

            }
			else //ak existuje twin
			{
				if (previouslyAccessedNext)
				{
                    //ak je v opacnom smere
                    if(currentHalfEdge->m_vertex != currentHalfEdge->m_twin->m_vertex)
                    {
                        currentHalfEdge = currentHalfEdge->m_twin->m_next;
						//currentHalfEdge = &(*currentHalfEdge->m_twin->m_next);
						//musim taktiez nastavit ze som ziskal tento currentHalfEdge
						//pomocou Nexta
						previouslyAccessedNext = true;
                    }
                    else
                    {
                        currentHalfEdge = currentHalfEdge->m_twin->m_previous;
						//currentHalfEdge = &(*currentHalfEdge->m_twin->m_previous);
						//musim taktiez nastavit ze som ziskal tento currentHalfEdge
						//pomocou Previousa
						previouslyAccessedNext = false;
                    }
				}
				else
				{
                    //ak je v opacnom smere
                    if(currentHalfEdge->m_vertex != currentHalfEdge->m_twin->m_vertex)
                    {
                        currentHalfEdge = currentHalfEdge->m_twin->m_previous;
						//currentHalfEdge = &(*currentHalfEdge->m_twin->m_previous);
						//musim taktiez nastavit ze som ziskal tento currentHalfEdge
						//pomocou Previousa
						previouslyAccessedNext = false;
                    }
					else //ak je v rovnakom smere
					{
                        currentHalfEdge = currentHalfEdge->m_twin->m_next;
						//currentHalfEdge = &(*currentHalfEdge->m_twin->m_next);
						//musim taktiez nastavit ze som ziskal tento currentHalfEdge
						//pomocou Nexta
						previouslyAccessedNext = true;
					}

				}
			}

        } while ((currentHalfEdge != startingHalfEdge) && !visitedTwoBoundaryEdges);

        return std::make_pair(returnedVerticesIndices, returnedFacesIndices);
    }
    

    void applyBrush(Ray& ray, float brushRadius, float brushStrength)
    {

    }

    void undo()
    {

    }

    static constexpr std::string_view getCommandName() noexcept { return "BasicSculptToolCommand"; }
};