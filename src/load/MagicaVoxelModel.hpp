//
// Created by georg on 5/16/2024.
//

#ifndef VOXEL_RAYMARCHER_MAGICAVOXELMODEL_HPP
#define VOXEL_RAYMARCHER_MAGICAVOXELMODEL_HPP

#include <cstdio>

// magic number
static int MV_ID( int a, int b, int c, int d ) {
    return ( a ) | ( b << 8 ) | ( c << 16 ) | ( d << 24 );
}

//================
// RGBA
//================
class MV_RGBA {
public :
    unsigned char r, g, b, a;
};

//================
// Voxel
//================
class MV_Voxel {
public :
    unsigned char x, y, z, colorIndex;
};

//================
// Model
//================
class MV_Model {
public :
    // size
    int size_x, size_y, size_z;

    // voxels
    int num_voxels;
    MV_Voxel *voxels;

    // palette
    bool is_custom_palette;
    MV_RGBA palette[ 256 ];



public :
    ~MV_Model() {
        Free();
    }

    MV_Model() :
            size_x(0 ),
            size_y(0 ),
            size_z(0 ),
            num_voxels(0 ),
            voxels( NULL ),
            is_custom_palette(false )
    {
    }

    void Free( void ) {
        if ( voxels ) {
            delete[] voxels;
            voxels = NULL;
        }
        num_voxels = 0;

        size_x = size_y = size_z = 0;

        is_custom_palette = false;

    }

    bool LoadModel( const char *path ) {
        // free old data
        Free();

        // open file
        FILE *fp = fopen( path, "rb" );
        if ( !fp  ){
            Error( "failed to open file" );
            return false;
        }

        // read file
        bool success = ReadModelFile( fp );

        // close file
        fclose( fp );

        // if failed, free invalid data
        if ( !success ) {
            Free();
        }

        return success;
    }

private :
    struct chunk_t {
        int id;
        int contentSize;
        int childrenSize;
        long end;
    };

private :
    bool ReadModelFile( FILE *fp ) {
        const int MV_VERSION = 150;

        const int ID_VOX  = MV_ID( 'V', 'O', 'X', ' ' );
        const int ID_MAIN = MV_ID( 'M', 'A', 'I', 'N' );
        const int ID_SIZE = MV_ID( 'S', 'I', 'Z', 'E' );
        const int ID_XYZI = MV_ID( 'X', 'Y', 'Z', 'I' );
        const int ID_RGBA = MV_ID( 'R', 'G', 'B', 'A' );

        // magic number
        int magic = ReadInt( fp );
        if ( magic != ID_VOX ) {
            Error( "magic number does not match" );
            return false;
        }

        // version
        ReadInt( fp );


        // main chunk
        chunk_t mainChunk;
        ReadChunk( fp, mainChunk );
        if ( mainChunk.id != ID_MAIN ) {
            Error( "main chunk is not found" );
            return false;
        }

        // skip content of main chunk
        fseek( fp, mainChunk.contentSize, SEEK_CUR );

        // read children chunks
        while ( ftell( fp ) < mainChunk.end ) {
            // read chunk header
            chunk_t sub;
            ReadChunk( fp, sub );

            if ( sub.id == ID_SIZE ) {
                // size
                size_x = ReadInt(fp );
                size_y = ReadInt(fp );
                size_z = ReadInt(fp );
            }
            else if ( sub.id == ID_XYZI ) {
                // num_voxels
                num_voxels = ReadInt(fp );
                if (num_voxels < 0 ) {
                    Error( "negative number of voxels" );
                    return false;
                }

                // voxels
                if (num_voxels > 0 ) {
                    voxels = new MV_Voxel[ num_voxels ];
                    fread(voxels, sizeof( MV_Voxel ), num_voxels, fp );
                }
            }
            else if ( sub.id == ID_RGBA ) {
                // last color is not used, so we only need to read 255 colors
                is_custom_palette = true;
                fread( palette + 1, sizeof( MV_RGBA ), 255, fp );

                // NOTICE : skip the last reserved color
                MV_RGBA reserved;
                fread( &reserved, sizeof( MV_RGBA ), 1, fp );
            }

            // skip unread bytes of current chunk or the whole unused chunk
            fseek( fp, sub.end, SEEK_SET );
        }


        return true;
    }

    void ReadChunk( FILE *fp, chunk_t &chunk ) {
        // read chunk
        chunk.id = ReadInt( fp );
        chunk.contentSize  = ReadInt( fp );
        chunk.childrenSize = ReadInt( fp );

        // end of chunk : used for skipping the whole chunk
        chunk.end = ftell( fp ) + chunk.contentSize + chunk.childrenSize;

        // print chunk info
        const char *c = ( const char * )( &chunk.id );

    }

    int ReadInt( FILE *fp ) {
        int v = 0;
        fread( &v, 4, 1, fp );
        return v;
    }

    void Error( const char *info ) const {
        printf( "[Error] MV_VoxelModel :: %s\n", info );
    }
};


#endif //VOXEL_RAYMARCHER_MAGICAVOXELMODEL_HPP
