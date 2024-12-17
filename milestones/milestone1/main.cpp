#include "runtime.h"
#include <iostream>
#include <memory>
#include <string>

int
main ()
{
  std::shared_ptr<Grid> grid = std::make_shared<Grid> ();
  std::shared_ptr<Runtime> runtime = std::make_shared<Runtime> (grid);

  grid->setCell (1, 1, std::move (std::make_unique<Integer> (2)), runtime);
  grid->setCell (2, 1, std::move (std::make_unique<Integer> (3)), runtime);
  grid->setCell (3, 1, std::move (std::make_unique<Integer> (4)), runtime);
  grid->setCell (4, 1, std::move (std::make_unique<Integer> (5)), runtime);

  grid->setCell (1, 2, std::move (std::make_unique<Integer> (1)), runtime);
  grid->setCell (2, 2, std::move (std::make_unique<Integer> (2)), runtime);
  grid->setCell (3, 2, std::move (std::make_unique<Float> (3)), runtime);
  grid->setCell (4, 2, std::move (std::make_unique<Integer> (4)), runtime);

  grid->setCell (
      2, 4,
      std::move (std::make_unique<Max> (std::make_unique<CellAddress> (1, 1),
                                        std::make_unique<CellAddress> (4, 2))),
      runtime); // #[2, 4] = max(#[1, 1], #[4, 2]) = 5

  grid->setCell (
      2, 5,
      std::move (std::make_unique<Min> (std::make_unique<CellAddress> (1, 1),
                                        std::make_unique<CellAddress> (4, 2))),
      runtime);

  grid->setCell (3, 4,
                 std::move (std::make_unique<Mean> (
                     std::make_unique<CellAddress> (1, 1),
                     std::make_unique<CellAddress> (4, 2))),
                 runtime);

  grid->setCell (
      3, 5,
      std::move (std::make_unique<Sum> (std::make_unique<CellAddress> (1, 1),
                                        std::make_unique<CellAddress> (4, 2))),
      runtime);

  std::unique_ptr<Expression> test1 = std::make_unique<Modulo> (
      std::make_unique<Add> (
          std::make_unique<Multiply> (std::make_unique<Integer> (7),
                                      std::make_unique<Integer> (4)),
          std::make_unique<Integer> (3)),
      std::make_unique<Integer> (12)); // (7 * 4 + 3) % 12

  std::unique_ptr<Expression> test2
      = std::make_unique<Not> (std::make_unique<GreaterThan> (
          std::make_unique<Float> (3.3),
          std::make_unique<Float> (3.2))); // !(3.3 > 3.2) = False

  std::unique_ptr<Expression> test3 = std::make_unique<Divide> (
      std::make_unique<IntToFloat> (std::make_unique<Integer> (7)),
      std::make_unique<Float> (2)); // float(7) / 2 = 3.5

  grid->setCell (0, 2, std::move (test1), runtime);
  grid->setCell (0, 1, std::move (test2), runtime);
  grid->setCell (0, 3, std::move (test3), runtime);

  grid->setCell (5, 5, std::move (std::make_unique<String> ("Hi")), runtime);

  grid->setCell (
      4, 5,
      std::move (std::make_unique<Multiply> (
          std::make_unique<RValue> (std::move (std::make_unique<Integer> (3)),
                                    std::move (std::make_unique<Integer> (1))),
          std::move (
              std::make_unique<Negation> (std::move (std::make_unique<RValue> (
                  std::move (std::make_unique<Integer> (2)),
                  std::move (std::make_unique<Integer> (1)))))))),
      runtime); // [4, 5] = (#[3, 1] * -(#[2, 1]))

  grid->setCell (5, 4,
                 std::move (std::make_unique<LeftShift> (
                     std::move (std::make_unique<RValue> (
                         std::move (std::make_unique<Add> (
                             std::move (std::make_unique<Integer> (1)),
                             std::make_unique<Integer> (1))),
                         std::move (std::make_unique<Integer> (1)))),
                     std::move (std::make_unique<Integer> (3)))),
                 runtime); // [5, 4] = #[(1 + 1), 1] << 3 = 24

  grid->setCell (5, 0,
                 std::move (std::make_unique<LessThan> (
                     std::move (std::make_unique<RValue> (
                         std::move (std::make_unique<Integer> (1)),
                         std::move (std::make_unique<Integer> (1)))),
                     std::move (std::make_unique<RValue> (
                         std::move (std::make_unique<Integer> (3)),
                         std::move (std::make_unique<Integer> (1)))))),
                 runtime); // [5, 0] = #[1, 1] < #[3, 1] = True

  grid->printGrid (runtime);
}