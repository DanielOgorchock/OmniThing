#ifndef OMNI_FIXEDARRAY_H
#define OMNI_FIXEDARRAY_H

namespace omni
{
    template<class T, unsigned short S>
    class FixedArray
    {
        private:
            T m_Array[S];
            unsigned short m_Count;

        public:
            FixedArray():
                m_Count(0)
            {

            }

            ~FixedArray()
            {

            }

            unsigned short getSize() const { return S; }
            unsigned short getCount() const { return m_Count; }

            const T& operator[] (unsigned int i) const
            {
                return m_Array[i];
            }

            T& operator[] (unsigned int i)
            {
                return m_Array[i];
            }

            bool addElement(const T& element)
            {
                if(m_Count >= S)
                    return false;

                m_Array[m_Count++] = element;
                return true;
            }

            bool removeElement(unsigned int index)
            {
                if(index >= S)
                    return false;

                for(unsigned short i = index; i < getCount()-1; ++i)
                {
                    m_Array[i] = m_Array[i+1];
                }

                --m_Count;

                return true;
            }

            void resetCount()
            {
                m_Count = 0; 
            }

    };
}

#endif
