#include <iostream>
#include <vtk-6.3/vtkImageData.h>
#include <vtk-6.3/vtkFloatArray.h>
#include <vtk-6.3/vtkShortArray.h>
#include <vtk-6.3/vtkInformation.h>
#include <vtk-6.3/vtkPointData.h>
#include <vtk-6.3/vtkLookupTable.h>
#include <vtk-6.3/vtkImageDataGeometryFilter.h>
#include <vtk-6.3/vtkPolyDataMapper.h>
#include <vtk-6.3/vtkRenderWindow.h>
#include <vtk-6.3/vtkActor.h>
#include <vtk-6.3/vtkRenderer.h>
#include <unistd.h>

const int MAP_X = 1350;
const int MAP_Y = 1350;


int main(int argc, char *argv[])
{
    short sdata[MAP_Y * MAP_X];
    int size[3] = {MAP_X, MAP_Y, 1}; /* 2 次元データ */
    FILE *fpi;

    /* ハードディスクからデータを読み込む */
    if ((fpi = fopen(argv[1], "rb")) == NULL) {
        puts("cannot open");
        exit(1);
    }
    fread(sdata, sizeof(short), MAP_X * MAP_Y, fpi);
    fclose(fpi);

    cout << "row1: " << sdata[0] << ", " << sdata[1]<< ", " << sdata[2]<< ", " << sdata[3] << ", " << "..." << endl;
    cout << "row2: " << sdata[MAP_X] << ", "<< sdata[MAP_X+1]<< ", " << sdata[MAP_X+2]<< ", " << sdata[MAP_X+3]<< ", " << "..." << endl;

    /* 読み込んだデータを sarray に代入 */
    auto sarray = vtkShortArray::New();
    sarray->SetArray(sdata, MAP_X * MAP_Y, 1);

    auto range = sarray->GetValueRange();
    cout << "min: " << range[0] << " max: " << range[1] << endl;

    /* 等間隔データを作成 */
    vtkImageData *imgData = vtkImageData::New();
    imgData->SetScalarType(VTK_TYPE_INT16, vtkInformation::New());
    imgData->SetDimensions(size);
    float sp = 0.1;
    imgData->SetSpacing(sp, sp, 0);

    /* 等間隔メッシュの箱に,データを代入 */
    imgData->GetPointData()->SetScalars(sarray);

    /* カラーテーブルを作成 */
    vtkLookupTable *lut = vtkLookupTable::New();
    lut->SetHueRange(0.667, 0.0); /* 青 -> 赤 */
    lut->Build();

    /* imgData を基にポリゴンデータを作成*/
    vtkImageDataGeometryFilter *igf
            = vtkImageDataGeometryFilter::New();
    igf->SetInputData(imgData);
    igf->Update();
//    igf->SetInput(imgData);

    vtkPolyDataMapper *Mapper = vtkPolyDataMapper::New();
//    Mapper->SetInput(igf->GetOutput());
    Mapper->SetInputData(igf->GetOutput());
    Mapper->SetLookupTable(lut);
    Mapper->SetColorModeToMapScalars();
    Mapper->SetScalarRange(range[0], range[1]);
//    Mapper->SetScalarRange(-500, 500);
    Mapper->SetScalarRange(-500, 3800);
    Mapper->Update();
    vtkActor *Actor = vtkActor::New();
    Actor->SetMapper(Mapper);
    vtkRenderer *vren = vtkRenderer::New();
    vren->AddActor(Actor);
    vren->SetBackground(0.0, 0.0, 0.0); /* 背景を黒に設定 */
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(vren);
    renWin->SetSize(600, 600); /*Window サイズを 600x600 に設定*/

    /* 10 秒程度表示 */
    // renWin->Render();
    // renWin->WaitForCompletion();
   for (int i = 0; i < 10; i++) {
       renWin->Render();
       sleep(1);
   }
    sarray->Delete();
    imgData->Delete();
    lut->Delete();
    Mapper->Delete();
    Actor->Delete();
    vren->Delete();
    renWin->Delete();
    return 0;
}
